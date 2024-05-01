package memorymap

type Module struct {
	Name      string
	Base      uint32
	Registers []Register
}

type Register interface {
	Name() string
	Offset() uint32
	Size() uint8
}

type RegisterBase struct {
	name   string
	offset uint32
}

type Register32 struct {
	RegisterBase
}

type Register16 struct {
	RegisterBase
}

var _ Register = Register32{}
var _ Register = Register16{}

func (r RegisterBase) Name() string {
	return r.name
}

func (r RegisterBase) Offset() uint32 {
	return r.offset
}

func (r Register16) Size() uint8 {
	return 2
}

func (r Register32) Size() uint8 {
	return 4
}
