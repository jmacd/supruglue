package arch

import (
	"fmt"
	"strconv"
	"strings"
)

type SystemEvent struct {
	EventNumber     string // Parsed as string, always int
	SignalName      string
	Source          string
	MIIRTSignalName string
}

func (se SystemEvent) Validate() error {
	if se.SignalName == "" {
		return fmt.Errorf("system event empty name: %s", se.Number)
	}
	if _, err := strconv.ParseInt(se.EventNumber, 10, 64); err != nil {
		return fmt.Errorf("system event range: %w", err)
	}

	return nil
}

func (se SystemEvent) Number() int8 {
	num, err := strconv.ParseInt(se.EventNumber, 10, 8)
	if err != nil {
		panic(err)
	}
	return int8(num)
}

func (se SystemEvent) CPPName() string {
	cppname := "SYSEVT_" + se.SignalName
	cppname = strings.ReplaceAll(cppname, "-", "_")
	cppname = strings.ReplaceAll(cppname, "[", "")
	cppname = strings.ReplaceAll(cppname, "]", "")
	cppname = strings.ReplaceAll(cppname, " ", "_")
	cppname = strings.ReplaceAll(cppname, "__", "_")
	cppname = strings.ToUpper(cppname)
	return cppname
}

func SystemEventMap(ses []SystemEvent) map[string]int8 {
	m := map[string]int8{}

	for _, se := range ses {
		m[se.CPPName()] = se.Number()
	}
	return m
}
