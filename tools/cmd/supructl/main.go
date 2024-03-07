package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"syscall"
	"unsafe"

	"github.com/jmacd/supruglue/arch/am335x"
	"github.com/jmacd/supruglue/tools/internal/arch"
	"github.com/jmacd/supruglue/tools/internal/csv"
	"github.com/jmacd/supruglue/tools/internal/firmware"
	"github.com/jmacd/supruglue/tools/internal/remoteproc"
	"github.com/jmacd/supruglue/tools/internal/rpmsghost"
	"github.com/spf13/cobra"
)

// rootCmd represents the base command when called without any subcommands
var (
	rootCmd = &cobra.Command{
		Use:   "supructl",
		Short: "Interacts with Supruglue PRU applications",
		Long:  "Interacts with Supruglue PRU applications",
	}

	flagFirmware      = rootCmd.PersistentFlags().StringP("firmware", "f", "", "firmware file")
	flagFirmwareDir   = rootCmd.PersistentFlags().StringP("firmware_dir", "d", "/lib/firmware", "firmware directory")
	flagRemoteProcDir = rootCmd.PersistentFlags().StringP("remoteproc", "p", "/sys/class/remoteproc/remoteproc1", "remoteproc")
)

func init() {
	rootCmd.AddCommand(journalCmd)
	rootCmd.AddCommand(startCmd)
	rootCmd.AddCommand(stopCmd)
	rootCmd.AddCommand(rodataCmd)
	rootCmd.AddCommand(dumpCmd)
}

var journalCmd = &cobra.Command{
	Use:   "journal",
	Short: "Print the Supruglue journal",
	RunE:  runJournal,
}

var startCmd = &cobra.Command{
	Use:   "start",
	Short: "Start running PRU firmware",
	Args:  cobra.NoArgs,
	RunE:  runStart,
}

var stopCmd = &cobra.Command{
	Use:   "stop",
	Short: "Stop running PRU firmware",
	Args:  cobra.NoArgs,
	RunE:  runStop,
}

var rodataCmd = &cobra.Command{
	Use:   "rodata",
	Short: "Print the rodata section of the PRU firmware",
	Args:  cobra.NoArgs,
	RunE:  runRodata,
}

var dumpCmd = &cobra.Command{
	Use:   "dump",
	Short: "Dump a register file",
	Args:  cobra.ExactArgs(1),
	RunE:  runDump,
}

func main() {
	err := rootCmd.Execute()
	if err != nil {
		os.Exit(1)
	}
}

func runStart(cmd *cobra.Command, _ []string) error {
	fw, err := firmware.Open(*flagFirmware)
	if err != nil {
		return fmt.Errorf("firmware: %w", err)
	}
	rp, err := remoteproc.Open(*flagRemoteProcDir, *flagFirmwareDir)
	if err != nil {
		return fmt.Errorf("remoteproc: %w", err)
	}
	if rp.State() != remoteproc.Offline {
		if err := rp.Stop(); err != nil {
			return fmt.Errorf("stop firmware: %w", err)
		}
	}
	if err := rp.Copy(fw); err != nil {
		return fmt.Errorf("copy firmware: %w", err)
	}
	if err := rp.SetFirmware(fw.Name()); err != nil {
		return fmt.Errorf("set firmware: %w", err)
	}
	if err := rp.Start(); err != nil {
		return fmt.Errorf("start firmware: %w", err)
	}

	app, err := rpmsghost.New(fw, rp)
	if err != nil {
		return fmt.Errorf("rpmsg host: %w", err)
	}

	return app.Run()
}

func runStop(cmd *cobra.Command, _ []string) error {
	rp, err := remoteproc.Open(*flagRemoteProcDir, *flagFirmwareDir)
	if err != nil {
		return fmt.Errorf("remoteproc: %w", err)
	}
	if err := rp.Stop(); err != nil {
		return fmt.Errorf("stop firmware: %w", err)
	}

	return nil
}

func runJournal(cmd *cobra.Command, args []string) error {
	fw, err := firmware.Open(*flagFirmware)
	if err != nil {
		return fmt.Errorf("firmware: %w", err)
	}
	rp, err := remoteproc.Open(*flagRemoteProcDir, *flagFirmwareDir)
	if err != nil {
		return fmt.Errorf("remoteproc: %w", err)
	}
	app, err := rpmsghost.New(fw, rp)
	if err != nil {
		return fmt.Errorf("rpmsg host: %w", err)
	}

	app.Run()
	select {}
}

func runRodata(cmd *cobra.Command, args []string) error {
	fwPath := *flagFirmware
	if fwPath == "" {
		rp, err := remoteproc.Open(*flagRemoteProcDir, *flagFirmwareDir)
		if err != nil {
			return fmt.Errorf("remoteproc: %w", err)
		}
		fwPath, err = rp.FirmwarePath()
		if err != nil {
			return err
		}
	}
	fmt.Println("firmware: ", fwPath)
	fw, err := firmware.Open(fwPath)
	if err != nil {
		return err
	}

	fw.ELF.Show()
	return nil
}

func abbrevHex(in string) uint32 {
	// remove trailing "h"
	u, err := strconv.ParseUint(in[:len(in)-1], 16, 32)
	if err != nil {
		panic(err)
	}
	return uint32(u)
}

func fromNum(in string) uint32 {
	u, err := strconv.ParseUint(in, 0, 32)
	if err != nil {
		panic(err)
	}
	return uint32(u)
}

func mmap(file *os.File, addr uint32, size int) ([]byte, error) {
	fmt.Printf("MMAP %x [%x] FD %d\n", addr, size, file.Fd())
	return syscall.Mmap(
		int(file.Fd()),
		int64(addr),
		size,
		syscall.PROT_READ|syscall.PROT_WRITE,
		syscall.MAP_SHARED,
	)
}
func runDump(cmd *cobra.Command, args []string) error {
	const listing = "am335x-map.csv"

	dump := args[0]

	fs := am335x.FS
	entries, err := fs.ReadDir(".")
	if err != nil {
		return err
	}

	modmap, err := fs.Open(listing)
	if err != nil {
		return err
	}

	allMods, err := csv.Read[arch.Module]("am335x-map.csv", modmap)
	if err != nil {
		return err
	}

	layouts := map[string][]arch.Register{}

	for _, ent := range entries {
		if ent.Name() == "am335x-map.csv" {
			continue
		}
		bn := strings.TrimSuffix(ent.Name(), ".csv")
		ef, err := fs.Open(ent.Name())
		if err != nil {
			return err
		}
		regs, err := csv.Read[arch.Register](bn, ef)
		if err != nil {
			return err
		}
		layouts[bn] = regs
	}

	var dumpMod arch.Module
	for _, mod := range allMods {
		_, ok := layouts[mod.Layout]
		if !ok {
			return fmt.Errorf("missing layout definition: %q", mod.Layout)
		}

		if mod.Module == dump {
			dumpMod = mod
		}
	}
	if dumpMod.Layout == "" {
		return fmt.Errorf("module not found: %q", dump)
	}

	layout := layouts[dumpMod.Layout]
	start := fromNum(dumpMod.Base)
	size := abbrevHex(layout[len(layout)-1].Offset) + fromNum(dumpMod.Unit)

	// TODO@@@
	size = 4096
	start = start & ^(size - 1)

	mem, err := os.OpenFile("/dev/mem", os.O_RDWR|os.O_SYNC, 0)
	if err != nil {
		return fmt.Errorf("open /dev/mem: %w", err)
	}

	ptr, err := mmap(mem, start, int(size))
	if err != nil {
		return fmt.Errorf("mmap /dev/mem: %w", err)
	}

	ints := (*[100]uint16)(unsafe.Pointer(&ptr[0]))

	// TODO!
	for i := uint32(0); i < (size / fromNum(dumpMod.Unit)); i++ {
		fmt.Printf("@%x: %x\n", i, ints[i])
	}

	return nil
}
