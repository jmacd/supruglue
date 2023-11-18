package rpmsghost

import (
	"encoding/binary"
	"fmt"
	"log"
	"os"
)

const deviceName = "/dev/rpmsg_pru30"

type RPMsgDevice struct {
	file *os.File
}

type Host struct {
	rpm *RPMsgDevice
}

func New() (*Host, error) {
	rpm, err := openRPMsgDevice()
	if err != nil {
		return nil, err
	}

	err = rpm.Write([]byte("hello"))
	if err != nil {
		return nil, err
	}

	return &Host{
		rpm: rpm,
	}, nil
}

func (host *Host) Run() {
	buf := make([]byte, 16)

	for {
		dat, err := host.rpm.Read(buf)
		if err != nil {
			log.Print(err)
			continue
		}
		// Interpret 4 32-bit words
		if len(dat) != 16 {
			log.Print(fmt.Errorf("data should be 16 bytes", len(dat)))
			continue
		}
		u0 := binary.LittleEndian.Uint32(dat[0:4])
		u1 := binary.LittleEndian.Uint32(dat[4:8])
		u2 := binary.LittleEndian.Uint32(dat[8:12])
		u3 := binary.LittleEndian.Uint32(dat[12:16])

		log.Printf("tid: %x; ptr: 0x%x; arg0: %d; arg1: %d\n", u0, u1, u2, u3)
	}
}

func openRPMsgDevice() (*RPMsgDevice, error) {
	file, err := os.OpenFile(deviceName, os.O_RDWR, 0666)
	return &RPMsgDevice{
		file: file,
	}, err
}

func (r *RPMsgDevice) Read(buf []byte) ([]byte, error) {
	n, err := r.file.Read(buf)
	if err != nil {
		return nil, err
	}
	return buf[:n], nil
}

func (r *RPMsgDevice) Write(data []byte) error {
	n, err := r.file.Write(data)
	if err != nil {
		return nil
	}
	if n != len(data) {
		return fmt.Errorf("short write: %d != %d", n, len(data))
	}
	return nil
}
