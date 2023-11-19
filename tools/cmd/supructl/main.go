package main

import (
	"fmt"
	"os"

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

	app, err := rpmsghost.New(fw)
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
	app, err := rpmsghost.New(fw)
	if err != nil {
		return fmt.Errorf("rpmsg host: %w", err)
	}

	app.Run()
	select {}
}
