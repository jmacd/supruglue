package rpmsghost

import (
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"strings"
	"time"

	"github.com/jmacd/supruglue/tools/internal/firmware"
)

const deviceName = "/dev/rpmsg_pru30"

type RPMsgDevice struct {
	file *os.File
}

type Host struct {
	rpm *RPMsgDevice
	fw  *firmware.Firmware
}

func New(fw *firmware.Firmware) (*Host, error) {
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
		fw:  fw,
	}, nil
}

const logEntrySize = 24

func (host *Host) Run() error {
	buf := make([]byte, logEntrySize)

	// TODO: clock correction, or similar
	fmt.Println("rpmsg: channel open")

	for {
		dat, err := host.rpm.Read(buf)
		if err != nil {
			log.Print(err)
			continue
		}
		// Interpret 4 32-bit words
		if len(dat) != cap(buf) {
			log.Print(fmt.Errorf("data should be %d bytes, was %d", logEntrySize, len(dat)))
			continue
		}
		u0 := binary.LittleEndian.Uint32(dat[0:4])
		u1 := binary.LittleEndian.Uint32(dat[4:8])
		u2 := binary.LittleEndian.Uint32(dat[8:12])
		u3 := binary.LittleEndian.Uint32(dat[12:16])
		u4 := binary.LittleEndian.Uint32(dat[16:20])
		u5 := binary.LittleEndian.Uint32(dat[20:24])

		msg, err := host.fw.ELF.CStringAt(uint64(u3))
		if err != nil || msg == "" {
			msg = fmt.Sprintf("<unknown msg 0x%x>", u3)
		} else {
			// TODO Should let %d coerce uint->int
			msg = strings.Replace(msg, "%u", "%d", -1)
			print := fmt.Sprintf(msg, u4, u5)
			if strings.Contains(print, "%!(EXTRA") {
				print = fmt.Sprintf(msg, u4)
			}
			if !strings.Contains(print, "%!(EXTRA") {
				msg = print
			}
		}
		elapsed := 5 * time.Duration(uint64(u2)<<32|uint64(u1))
		ts := elapsed.String()

		fmt.Printf("%s [%05x] %s\n", ts, u0, msg)
	}
}

const retryDelay = 100 * time.Millisecond

func openRPMsgDevice() (*RPMsgDevice, error) {
	const numTries = 10
	var err error
	for tries := 0; tries < numTries; tries++ {
		if err != nil {
			fmt.Println("rpmsg:", err)
		}
		var file *os.File
		file, err = os.OpenFile(deviceName, os.O_RDWR, 0666)
		if err != nil {
			time.Sleep(retryDelay)
			continue
		}
		return &RPMsgDevice{
			file: file,
		}, err
	}
	return nil, fmt.Errorf("too many retries: %w", err)
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
