package am335x

import (
	"embed"
	_ "embed"
)

//go:embed *.csv
var FS embed.FS
