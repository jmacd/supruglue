package main

import (
	"fmt"
	"log"
	"os"
)

const deviceName = "/dev/rpmsg_pru30"

type RPMsgDevice struct {
	file *os.File
}

type appState struct {
	rpm *RPMsgDevice
}

func newAppState() (*appState, error) {
	rpm, err := openRPMsgDevice()
	if err != nil {
		return nil, err
	}

	// if err := rpm.write([]byte("wakeup")); err != nil {
	// 	return nil, err
	// }

	return &appState{
		rpm: rpm,
	}, nil
}

func (app *appState) run() error {
	buf := make([]byte, 4096)
	for {
		dat, err := app.rpm.read(buf)
		if err != nil {
			log.Print(err)
		}
		log.Print("LOOK", string(dat))
	}
}

func openRPMsgDevice() (*RPMsgDevice, error) {
	file, err := os.OpenFile(deviceName, os.O_RDWR, 0666)
	return &RPMsgDevice{
		file: file,
	}, err
}

func (r *RPMsgDevice) read(buf []byte) ([]byte, error) {
	n, err := r.file.Read(buf)
	if err != nil {
		return nil, err
	}
	return buf[:n], nil
}

func (r *RPMsgDevice) write(data []byte) error {
	n, err := r.file.Write(data)
	if err != nil {
		return nil
	}
	if n != len(data) {
		return fmt.Errorf("short write: %d != %d", n, len(data))
	}
	return nil
}

func main() {
	app, err := newAppState()
	if err != nil {
		panic(err)
	}
	app.run()
	// go app.run()
	// select {}
}
