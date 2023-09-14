package arch

import (
	"fmt"
	"strconv"
	"strings"
)

type Pin struct {
	Name    string
	Addr    string
	Mode0   string
	Mode1   string
	Mode2   string
	Mode3   string
	Mode4   string
	Mode5   string
	Mode6   string
	Mode7   string
	GpioNum string
	Name2   string
}

func (p Pin) Validate() error {
	if p.Name != p.Name2 {
		return fmt.Errorf("name mismatch: %q != %q", p.Name, p.Name2)
	}

	return nil
}

func (p Pin) HasGpio() bool {
	return strings.HasPrefix(p.Mode7, "gpio")
}

func (p Pin) GpioBank() (int, error) {
	return strconv.Atoi(p.Mode7[4:5])
}

func (p Pin) GpioBit() (int, error) {
	return strconv.Atoi(p.Mode7[6:])
}

func (p Pin) PRUHasGpioFastOutput() (int, bool) {
	if strings.HasPrefix(p.Mode5, "pr1_pru0_pru_r30_") {
		return 0, true
	}
	if strings.HasPrefix(p.Mode5, "pr1_pru1_pru_r30_") {
		return 1, true
	}
	return -1, false
}

func (p Pin) PRUGpioFastOutputBit() (int, error) {
	return strconv.Atoi(p.Mode5[len("pr1_pruX_pru_r30_"):])
}

func (p Pin) PRUHasGpioFastInput() (int, bool) {
	if strings.HasPrefix(p.Mode6, "pr1_pru0_pru_r31_") {
		return 0, true
	}
	if strings.HasPrefix(p.Mode6, "pr1_pru1_pru_r31_") {
		return 1, true
	}
	return -1, false
}

func (p Pin) PRUGpioFastInputBit() (int, error) {
	return strconv.Atoi(p.Mode6[len("pr1_pruX_pru_r31_"):])
}
