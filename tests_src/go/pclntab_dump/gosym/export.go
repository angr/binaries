// Exporter over the vendored go1.16.15 debug/gosym (pclntab.go, symtab.go are verbatim copies):
// the raw _func words and the decoded pcsp table of every function, for cross-checking cle.

package gosym

// RawFunc is one functab entry with the nine uint32 words that follow _func.entry, read without
// interpretation (their meaning depends on the Go version), and the pcsp table decoded by step.
type RawFunc struct {
	Name   string
	Entry  uint64
	End    uint64
	Words  [9]uint32
	PCSP   [][2]int64 // (pc offset from entry, sp delta)
	File   string
	Line   int
	NFiles uint32
}

func (t *LineTable) Version() string {
	t.parsePclnTab()
	switch t.version {
	case ver12:
		return "1.2"
	case ver116:
		return "1.16"
	}
	return "?"
}

func (t *LineTable) RawFuncs() []RawFunc {
	t.parsePclnTab()
	n := len(t.functab) / int(t.ptrsize) / 2
	out := make([]RawFunc, n)
	for i := range out {
		f := &out[i]
		f.Entry = t.uintptr(t.functab[2*i*int(t.ptrsize):])
		f.End = t.uintptr(t.functab[(2*i+2)*int(t.ptrsize):])
		info := t.funcdata[t.uintptr(t.functab[(2*i+1)*int(t.ptrsize):]):]
		for w := range f.Words {
			f.Words[w] = t.binary.Uint32(info[int(t.ptrsize)+4*w:])
		}
		f.Name = t.funcName(f.Words[0])
		if off := f.Words[3]; off != 0 {
			p := t.pctab[off:]
			pc, val, first := f.Entry, int32(-1), true
			for {
				prev := pc
				if !t.step(&p, &pc, &val, first) {
					break
				}
				first = false
				f.PCSP = append(f.PCSP, [2]int64{int64(prev - f.Entry), int64(val)})
			}
		}
		f.File = t.go12PCToFile(f.Entry)
		f.Line = t.go12PCToLine(f.Entry)
		f.NFiles = t.nfiletab
	}
	return out
}
