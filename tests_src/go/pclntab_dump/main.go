// Dumps what Go's own debug/gosym (go1.16.15's copy, which reads the Go 1.2 and 1.16 table layouts)
// makes of a binary's pclntab, as JSON. The cle tests' expected values for the pre-1.18 fixtures
// come from here; run it with a go1.16 toolchain:
//
//	cd tests_src/go/pclntab_dump && go run . ../../../tests/x86_64/go/go1.15.15/basics main.main main.fib
//
// Without function names it prints every function.
package main

import (
	"bytes"
	"debug/elf"
	"debug/macho"
	"debug/pe"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"os"
	"strings"

	"./gosym"
)

func pclntab(path string) ([]byte, error) {
	if f, err := elf.Open(path); err == nil {
		defer f.Close()
		if s := f.Section(".gopclntab"); s != nil {
			return s.Data()
		}
		return nil, fmt.Errorf("no .gopclntab")
	}
	if f, err := macho.Open(path); err == nil {
		defer f.Close()
		if s := f.Section("__gopclntab"); s != nil {
			return s.Data()
		}
		return nil, fmt.Errorf("no __gopclntab")
	}
	f, err := pe.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	// no section of its own: locate the table by its magic (first hit that gosym accepts)
	for _, s := range f.Sections {
		data, err := s.Data()
		if err != nil {
			continue
		}
		for _, magic := range [][]byte{{0xfb, 0xff, 0xff, 0xff, 0, 0}, {0xfa, 0xff, 0xff, 0xff, 0, 0}} {
			for pos := bytes.Index(data, magic); pos != -1; pos = bytes.Index(data[pos+1:], magic) + pos + 1 {
				t := gosym.NewLineTable(data[pos:], 0)
				if t.Version() != "?" && len(t.RawFuncs()) > 0 {
					return data[pos:], nil
				}
			}
		}
	}
	return nil, fmt.Errorf("no pclntab magic found")
}

func main() {
	data, err := pclntab(os.Args[1])
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	want := map[string]bool{}
	for _, name := range os.Args[2:] {
		want[name] = true
	}
	t := gosym.NewLineTable(data, 0)
	funcs := t.RawFuncs()
	out := map[string]interface{}{
		"version":  t.Version(),
		"magic":    fmt.Sprintf("%#x", binary.LittleEndian.Uint32(data)),
		"min_lc":   data[6],
		"ptr_size": data[7],
		"nfunc":    len(funcs),
	}
	if len(funcs) > 0 {
		out["nfiles"] = funcs[0].NFiles
	}
	selected := map[string]gosym.RawFunc{}
	for _, f := range funcs {
		if len(want) == 0 || want[f.Name] {
			selected[f.Name] = f
		}
	}
	out["functions"] = selected
	enc := json.NewEncoder(os.Stdout)
	enc.SetIndent("", " ")
	if err := enc.Encode(out); err != nil {
		fmt.Fprintln(os.Stderr, strings.TrimSpace(err.Error()))
		os.Exit(1)
	}
}
