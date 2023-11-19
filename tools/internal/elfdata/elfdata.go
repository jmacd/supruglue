package elfdata

import (
	"debug/elf"
	"fmt"
	"path"
	"strings"
)

// Presently this indexes the .rodata section, which is where clpru
// places string constants.
type ELF struct {
	addr   uint64
	rodata []byte
}

func Open(fw string) (*ELF, error) {
	base := path.Base(fw)
	f, err := elf.Open(fw)
	if err != nil {
		return nil, err
	}

	for _, sect := range f.Sections {
		if sect.Type != elf.SHT_PROGBITS || sect.Size == 0 || strings.HasPrefix(sect.Name, ".debug") {
			continue
		}
		fmt.Printf("%s: %s: %d bytes\n", base, sect.Name, sect.Size)
	}

	sect := f.Section(".rodata")
	if sect == nil {
		return nil, fmt.Errorf("no .rodata section in firmware")
	}

	rodata, err := sect.Data()
	if err != nil {
		return nil, fmt.Errorf("reading .rodata: %w", err)
	}

	return &ELF{
		addr:   sect.Addr,
		rodata: rodata,
	}, nil
}

func (sd *ELF) CStringAt(addr uint64) (string, error) {
	if addr < sd.addr || (addr-sd.addr) >= uint64(len(sd.rodata)) {
		return "", fmt.Errorf("address range [%d,%d)", sd.addr, sd.addr+uint64(len(sd.rodata)))
	}

	off := int(addr - sd.addr)
	var b []byte

	for idx := 0; idx < off; idx++ {
		if sd.rodata[idx] == 0 {
			break
		}
		b = append(b, sd.rodata[idx])
	}
	return string(b), nil
}
