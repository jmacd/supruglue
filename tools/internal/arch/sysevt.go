package arch

import (
	"fmt"
	"strconv"
)

type SysEvt struct {
	Number          string // Parsed as string, always int
	SignalName      string
	Source          string
	MIIRTSignalName string
}

func (p SysEvt) Validate() error {
	if p.SignalName == "" {
		return fmt.Errorf("system event empty name: %s", p.Number)
	}
	if _, err := strconv.ParseInt(p.Number, 10, 64); err != nil {
		return fmt.Errorf("system event range: %w", err)
	}

	return nil
}
