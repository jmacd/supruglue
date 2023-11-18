package elfdata

import (
	"debug/elf"
	"fmt"
)

// Presently this indexes the .rodata section, which is where clpru
// places string constants.
type Strings struct {
	addr uint64
	data []byte
}

func Open(fw string) (*Strings, error) {
	f, err := elf.Open(fw)
	if err != nil {
		return nil, err
	}
	sect := f.Section(".rodata")
	if sect == nil {
		return nil, fmt.Errorf("no .rodata section in firmware")
	}

	data, err := sect.Data()
	if err != nil {
		return nil, fmt.Errorf("reading .rodata: %w", err)
	}

	return &Strings{
		addr: sect.Addr,
		data: data,
	}, nil
}

func (sd *Strings) CStringAt(addr uint64) (string, error) {
	if addr < sd.addr || (addr-sd.addr) >= uint64(len(sd.data)) {
		return "", fmt.Errorf("address range [%d,%d)", sd.addr, sd.addr+uint64(len(sd.data)))
	}

	off := int(addr - sd.addr)
	var b []byte

	for idx := 0; idx < off; idx++ {
		if sd.data[idx] == 0 {
			break
		}
		b = append(b, sd.data[idx])
	}
	return string(b), nil
}
