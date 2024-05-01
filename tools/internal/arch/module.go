package arch

type Module struct {
	Module string
	Base   string
	Unit   string
	Layout string
}

func (r Module) Validate() error {
	return nil
}
