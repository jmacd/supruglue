package main

import (
	"fmt"
	"os"

	"github.com/jmacd/supruglue/tools/internal/elfdata"
	"github.com/jmacd/supruglue/tools/internal/rpmsghost"
	"github.com/spf13/cobra"
)

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "supructl",
	Short: "Interacts with Supruglue PRU applications",
	Long:  "Interacts with Supruglue PRU applications",
}

var journalCmd = &cobra.Command{
	Use:   "journal",
	Short: "Print the Supruglue journal",
	Run: func(cmd *cobra.Command, args []string) {
		app, err := rpmsghost.New()
		if err != nil {
			panic(err)
		}

		app.Run()
	},
}

// e.g.,
// ./bazel-bin/tools/cmd/supructl/supructl_/supructl strings ./bazel-out/pru-fastbuild/bin/examples/example_pru0
var stringsCmd = &cobra.Command{
	Use:   "strings",
	Short: "Print strings from a PRU firmware",
	RunE: func(cmd *cobra.Command, args []string) error {
		if len(args) != 1 {
			return fmt.Errorf("usage: strings <firmware>")
		}
		rodata, err := elfdata.Open(args[0])
		if err != nil {
			return fmt.Errorf("elf data: %w", err)
		}

		str, err := rodata.CStringAt(0x36b0)
		if err != nil {
			return fmt.Errorf("elf string: %w", err)
		}
		fmt.Println(str)
		return nil
	},
}

func init() {
	rootCmd.AddCommand(journalCmd)
	rootCmd.AddCommand(stringsCmd)
}

func main() {
	err := rootCmd.Execute()
	if err != nil {
		os.Exit(1)
	}
}
