package firmware

import (
	"fmt"
	"path"

	"github.com/jmacd/supruglue/tools/internal/elfdata"
)

type Firmware struct {
	full string
	*elfdata.ELF
}

func Open(full string) (*Firmware, error) {
	if full == "" {
		return nil, fmt.Errorf("firmware name is empty")
	}
	data, err := elfdata.Open(full)
	if err != nil {
		return nil, fmt.Errorf("elf data: %w", err)
	}
	fw := &Firmware{
		full: full,
		ELF:  data,
	}
	return fw, nil
}

func (fw *Firmware) Path() string {
	return fw.full
}

func (fw *Firmware) Name() string {
	return path.Base(fw.full)
}
