package arch

import (
	"fmt"
)

type SysEvt struct {
	Number          int
	SignalName      string
	Source          string
	MIIRTSignalName string
}

func (p SysEvt) Validate() error {
	if p.SignalName == "" {
		return fmt.Errorf("system event empty name: %d", p.Number)
	}
	if p.Number < 0 || p.Number > 63 {
		return fmt.Errorf("system event range: %s: %d", p.SignalName, p.Number)
	}

	return nil
}
