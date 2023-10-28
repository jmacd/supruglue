package arch

type IRQ struct {
	Event   string `mapstructure:"event"`
	Channel int    `mapstructure:"channel"`
	Host    int    `mapstructure:"host"`
	Desc    string `mapstructure:"desc"`
}

type IRQs struct {
	Incoming []IRQ `mapstructure:"incoming"`
}
