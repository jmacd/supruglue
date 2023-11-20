package remoteproc

import (
	"fmt"
	"io"
	"os"
	"path"
	"strings"

	"github.com/jmacd/supruglue/tools/internal/firmware"
)

type Property string

const (
	stateProp Property = "state"
	fwareProp Property = "firmware"

	tmpBase = "supru"
)

type State string

const (
	Offline State = "offline"
	Running State = "running"
)

type RemoteProc struct {
	rpDir string
	fwDir string
}

func (rp *RemoteProc) readProp(prop Property) string {
	full := path.Join(rp.rpDir, string(prop))
	data, err := os.ReadFile(full)
	if err != nil {
		fmt.Printf("remoteproc: %s: %v\n", full, err)
		return ""
	}
	return strings.TrimSpace(string(data))
}

func (rp *RemoteProc) setProp(prop Property, value string) error {
	full := path.Join(rp.rpDir, string(prop))
	return os.WriteFile(full, []byte(value), 0666)
}

func openDir(dir string) error {
	df, err := os.Open(dir)
	if err != nil {
		return err
	}
	defer df.Close()
	st, err := df.Stat()
	if err != nil {
		return err
	}
	if !st.IsDir() {
		return fmt.Errorf("not a dir: %s", dir)
	}
	return nil
}

func Open(rpDir, fwDir string) (*RemoteProc, error) {
	rp := &RemoteProc{
		rpDir: rpDir,
		fwDir: fwDir,
	}
	if err := openDir(rpDir); err != nil {
		return nil, err
	}
	if err := openDir(fwDir); err != nil {
		return nil, err
	}
	fmt.Println("remoteproc: opened ", rpDir)
	for _, prop := range []Property{stateProp, fwareProp} {
		fmt.Printf("remoteproc: %s = %q\n", prop, rp.readProp(prop))
	}
	return rp, nil
}

func (rp *RemoteProc) Copy(fw *firmware.Firmware) error {
	dst, err := os.CreateTemp(rp.fwDir, tmpBase)
	if err != nil {
		return fmt.Errorf("temp firmware: %s: %w", rp.fwDir, err)
	}
	fwFile, err := os.Open(fw.Path())
	if err != nil {
		return fmt.Errorf("open firmware: %s: %w", fw.Path(), err)
	}
	defer fwFile.Close()

	if _, err := io.Copy(dst, fwFile); err != nil {
		return fmt.Errorf("copy firmware: %w", err)
	}
	if err := dst.Close(); err != nil {
		return fmt.Errorf("close firmware: %w", err)
	}
	if err := os.Rename(dst.Name(), path.Join(rp.fwDir, fw.Name())); err != nil {
		return fmt.Errorf("rename firmware: %w", err)
	}
	return nil
}

func (rp *RemoteProc) State() State {
	return State(rp.readProp(stateProp))
}

func (rp *RemoteProc) Stop() error {
	fmt.Println("remoteproc: stopping", rp.readProp(fwareProp))
	return rp.setProp(stateProp, "stop")
}

func (rp *RemoteProc) Start() error {
	fmt.Println("remoteproc: starting", rp.readProp(fwareProp))
	return rp.setProp(stateProp, "start")
}

func (rp *RemoteProc) Firmware() string {
	return rp.readProp(fwareProp)
}

func (rp *RemoteProc) FirmwarePath() (string, error) {
	fw := rp.Firmware()
	if fw == "" {
		return "", fmt.Errorf("no firmware is set")
	}
	return path.Join(rp.fwDir, fw), nil
}

func (rp *RemoteProc) SetFirmware(name string) error {
	return rp.setProp(fwareProp, name)
}
