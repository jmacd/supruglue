package fwstate

import (
	"fmt"
	"regexp"
	"sort"
	"sync"
	"time"
)

type metricEntry struct {
	ts  time.Duration
	val uint64
}

type Metrics struct {
	lock  sync.Mutex
	state map[string]map[string]*[2]metricEntry
}

type noopFmt struct{}

func (noopFmt) Format(f fmt.State, verb rune) {
	_, _ = f.Write([]byte("{}"))
}

func NewMetrics() *Metrics {
	return &Metrics{
		state: map[string]map[string]*[2]metricEntry{},
	}
}

var mre = regexp.MustCompile(`[a-zA-Z0-9_\.]+={}`)

func (m *Metrics) Update(ts time.Duration, name, msg string, flags uint32, a, b uint64) {
	m.lock.Lock()
	defer m.lock.Unlock()

	cnt := 0
	if (flags & 0x3000) != 0 {
		cnt++
	}
	if (flags & 0x3000) != 0 {
		cnt++
	}
	var tfmt string
	if cnt == 1 {
		tfmt = fmt.Sprintf(msg, noopFmt{})
	} else {
		tfmt = fmt.Sprintf(msg, noopFmt{}, noopFmt{})
	}
	matches := mre.FindAllString(tfmt, -1)
	if len(matches) != cnt {
		panic("unhandled")
	}
	if 0 < cnt {
		m.update1(ts, name, matches[0][:len(matches[0])-3], a)
	}
	if 1 < cnt {
		m.update1(ts, name, matches[1][:len(matches[1])-3], b)
	}
}

func (m *Metrics) update1(ts time.Duration, tname, mname string, val uint64) {
	tm, ok := m.state[tname]
	if !ok {
		tm = map[string]*[2]metricEntry{}
		m.state[tname] = tm
	}
	mm, ok := tm[mname]
	if !ok {
		mm = new([2]metricEntry)
		tm[mname] = mm
	}
	mm[0] = mm[1]
	mm[1].ts = ts
	mm[1].val = val
}

func (m *Metrics) Show() {
	m.lock.Lock()
	defer m.lock.Unlock()

	var tnames []string
	for tname := range m.state {
		tnames = append(tnames, tname)
	}
	sort.Strings(tnames)

	for _, tname := range tnames {
		fmt.Printf("  %s\n", tname)

		for mname, vals := range m.state[tname] {
			fmt.Printf("    %s: %f%%\n", mname, 100*float64(time.Second)*float64(vals[1].val-vals[0].val)/float64(vals[1].ts-vals[0].ts)/200e9)
		}
	}

}
