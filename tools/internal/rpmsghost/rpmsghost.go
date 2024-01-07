package rpmsghost

import (
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"path"
	"strings"
	"time"

	"github.com/jmacd/supruglue/tools/internal/firmware"
	"github.com/jmacd/supruglue/tools/internal/fwstate"
	"github.com/jmacd/supruglue/tools/internal/remoteproc"
)

const logEntrySize = 36

type RPMsgDevice struct {
	file *os.File
}

type Host struct {
	rpm *RPMsgDevice
	fw  *firmware.Firmware
	ms  *fwstate.Metrics
}

func deviceName(rp *remoteproc.RemoteProc) string {
	if path.Base(rp.Directory()) == "remoteproc1" {
		return "/dev/rpmsg_pru30"
	}
	return "/dev/rpmsg_pru31"
}

func New(fw *firmware.Firmware, rp *remoteproc.RemoteProc) (*Host, error) {
	rpm, err := openRPMsgDevice(rp)
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
		ms:  fwstate.NewMetrics(),
	}, nil
}

func (host *Host) Run() error {
	buf := make([]byte, logEntrySize*2)

	// TODO: clock correction, or similar
	fmt.Println("rpmsg: channel open")

	go func() {
		t := time.NewTicker(time.Second * 5)
		defer t.Stop()
		for {
			select {
			case <-t.C:
				host.ms.Show()
			}
		}
	}()

	for {
		dat, err := host.rpm.Read(buf)
		if err != nil {
			log.Print(err)
			continue
		}

		// Interpret 4 32-bit words
		if len(dat) != logEntrySize {
			log.Print(fmt.Errorf("data should be %d bytes, was %d", logEntrySize, len(dat)))
			continue
		}
		tid := binary.LittleEndian.Uint32(dat[0:4])
		flags := binary.LittleEndian.Uint32(dat[4:8])
		tslow := binary.LittleEndian.Uint32(dat[8:12])
		tshigh := binary.LittleEndian.Uint32(dat[12:16])
		msgptr := binary.LittleEndian.Uint32(dat[16:20])

		var a, b uint64

		switch {
		case flags&0x1000 != 0:
			a = uint64(binary.LittleEndian.Uint32(dat[24:28]))
			fmt.Println("A1", a)
		case flags&0x2000 != 0:
			a = uint64(binary.LittleEndian.Uint32(dat[20:24]))
			a |= uint64(binary.LittleEndian.Uint32(dat[24:28])) << 32
			fmt.Println("A2", a)
		}
		switch {
		case flags&0x4000 != 0:
			b = uint64(binary.LittleEndian.Uint32(dat[32:36]))
			fmt.Println("B1", b)
		case flags&0x8000 != 0:
			b = uint64(binary.LittleEndian.Uint32(dat[28:32]))
			b |= uint64(binary.LittleEndian.Uint32(dat[32:36])) << 32
			fmt.Println("B2", b)
		}

		msg, err := host.fw.ELF.CStringAt(uint64(msgptr))
		if err != nil || msg == "" {
			msg = fmt.Sprintf("<unknown msg 0x%x>", msg)
		}

		elapsed := 5 * time.Duration(uint64(tshigh)<<32|uint64(tslow))
		tname := ""
		if name, err := host.fw.ELF.ThreadNameAt(uint64(tid)); err == nil {
			tname = name
		} else {
			tname = fmt.Sprintf("%05x", tid)
		}

		if (flags & 0x10) != 0 {
			host.ms.Update(elapsed, tname, msg, flags, a, b)
			continue
		}

		msg = strings.Replace(msg, "%u", "%d", -1)
		print := fmt.Sprintf(msg, a, b)
		if strings.Contains(print, "%!(EXTRA") {
			print = fmt.Sprintf(msg, a)
		}
		if !strings.Contains(print, "%!(EXTRA") {
			msg = print
		}

		ts := elapsed.String()

		fmt.Printf("%s [%s] %s\n", ts, tname, msg)
	}
}

const retryDelay = 100 * time.Millisecond

func openRPMsgDevice(rp *remoteproc.RemoteProc) (*RPMsgDevice, error) {
	const numTries = 10
	var err error
	for tries := 0; tries < numTries; tries++ {
		if err != nil {
			fmt.Println("rpmsg:", err)
		}
		var file *os.File
		file, err = os.OpenFile(deviceName(rp), os.O_RDWR, 0666)
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
