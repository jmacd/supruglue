package elfdata

import (
	"debug/elf"
	"fmt"
	"path"
	"strings"
	"unicode"
)

// Presently this indexes the .rodata section, which is where clpru
// places string constants.
type ELF struct {
	addr   uint64
	rodata []byte

	threads map[uint64]string
}

func Open(fw string) (*ELF, error) {
	base := path.Base(fw)
	f, err := elf.Open(fw)
	if err != nil {
		return nil, err
	}

	threads := map[uint64]string{}

	for _, sect := range f.Sections {
		switch {
		case sect.Size == 0:
			continue
		case strings.HasPrefix(sect.Name, ".debug_"):
			continue
		case strings.HasPrefix(sect.Name, ".TI."):
			continue
		case strings.HasPrefix(sect.Name, ".creg."):
			continue
		case strings.HasPrefix(sect.Name, "__TI_"):
			continue
		case map[string]bool{
			".strtab":   true,
			".symtab":   true,
			".shstrtab": true,
		}[sect.Name]:
			continue
		}

		if strings.HasPrefix(sect.Name, ".thread.") {
			threads[sect.Addr] = sect.Name[len(".thread."):]
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
		addr:    sect.Addr,
		rodata:  rodata,
		threads: threads,
	}, nil
}

func (elf *ELF) Show() {
	var str strings.Builder
	appf := func(msg string, args ...any) {
		str.WriteString(fmt.Sprintf(msg, args...))
	}
	// like `hexdump -C`
	base := elf.addr &^ 0xf
	limit := (elf.addr + uint64(len(elf.rodata)) + 0xf) &^ 0xf

	for base < limit {
		var line strings.Builder
		appf("[%05x]  ", base)
		for col := 0; col < 0x10; col++ {
			if base >= elf.addr || (base-elf.addr) < uint64(len(elf.rodata)) {
				b := elf.rodata[base-elf.addr]
				appf("%02x ", b)
				if unicode.IsPrint(rune(b)) {
					line.WriteRune(rune(b))
				} else {
					line.WriteRune('☐')
				}
			} else {
				appf("   ")
				line.WriteRune(' ')
			}
			base++
		}
		appf(" |")
		appf("%s", line.String())
		appf("|\n")
	}
	fmt.Println(str.String())
}

func (elf *ELF) CStringAt(addr uint64) (string, error) {
	if addr < elf.addr || (addr-elf.addr) >= uint64(len(elf.rodata)) {
		return "", fmt.Errorf("address range [%d,%d)", elf.addr, elf.addr+uint64(len(elf.rodata)))
	}

	var b []byte
	for off := int(addr - elf.addr); off < len(elf.rodata); off++ {
		if elf.rodata[off] == 0 {
			break
		}
		b = append(b, elf.rodata[off])
	}
	return string(b), nil
}

func (elf *ELF) ThreadNameAt(addr uint64) (string, error) {
	name, ok := elf.threads[addr]
	if !ok {
		return "", fmt.Errorf("no thread at this address: %u", addr)
	}
	return name, nil
}
