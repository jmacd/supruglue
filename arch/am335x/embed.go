package am335x

import (
	"embed"
	"fmt"
)

//go:embed *.csv
var FS embed.FS

func helper() {
	for i := 0; i < 64; i++ {
		fmt.Printf("%Xh,DCHMAP_%d,DMA Channel Mapping Register %d\n", i*4+0x100, i, i)
	}
	for i := 0; i < 8; i++ {
		fmt.Printf("%Xh,QCHMAP_%d,QDMA Channel Mapping Register %d\n", i*4+0x200, i, i)
	}
	for i := 0; i < 8; i++ {
		fmt.Printf("%Xh,DMAQNUM_%d,DMA Queue Number Register %d\n", i*4+0x240, i, i)
	}
	for i := 0; i < 8; i++ {
		fmt.Printf("%Xh,QRAE_%d,QDMA Region access Enable Register for Region %d\n", i*4+0x380, i, i)
	}
	for q := 0; q < 3; q++ {
		for i := 0; i < 16; i++ {
			fmt.Printf("%Xh,Q%dE%d,Event Queue %d Entry %d\n", (q*16+i)*4+0x400, q, i, q, i)
		}
	}
	for i := 0; i < 8; i++ {
		fmt.Printf("%Xh,MPPA_%d,Global Memory Protection Page Attribute Register for Region %d\n", i*4+0x810, i, i)
	}
}
