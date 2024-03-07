package arch

import (
	"fmt"
	"strings"
)

type Register struct {
	Offset      string
	Name        string
	Description string
}

func (r Register) Validate() error {
	if !strings.HasSuffix(r.Offset, "h") {
		return fmt.Errorf("should have h suffix: %q", r.Offset)
	}

	return nil
}
