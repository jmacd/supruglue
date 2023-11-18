package main

import (
	"github.com/jmacd/supruglue/tools/internal/rpmsghost"
	// "github.com/spf13/cobra"
)

func main() {
	app, err := rpmsghost.New()
	if err != nil {
		panic(err)
	}

	go app.Run()

	select {}
}
