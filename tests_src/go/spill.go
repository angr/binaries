// Multi-word values (a slice, a string, an error) that live in stack memory across calls (their address is taken):
// the function returns them from the stack words, so a decompiler sees the header word by word unless it recovers
// the value.
package main

import (
	"errors"
	"os"
	"strconv"
)

//go:noinline
func piece(i int) ([]byte, error) {
	if i > 3 {
		return nil, errors.New("too many")
	}
	return []byte(strconv.Itoa(i)), nil
}

//go:noinline
func keep(p *[]byte) {
	if *p == nil {
		*p = []byte{}
	}
}

//go:noinline
func gather(n int) ([]byte, error) {
	var out []byte
	keep(&out)
	for i := 0; i < n; i++ {
		b, err := piece(i)
		if err != nil {
			return out, err
		}
		out = append(out, b...)
	}
	return out, nil
}

//go:noinline
func name(i int) (string, error) {
	if i < 0 {
		return "", errors.New("negative")
	}
	return "n" + strconv.Itoa(i), nil
}

//go:noinline
func keepString(p *string) {
	if *p == "" {
		*p = "-"
	}
}

//go:noinline
func joined(n int) (string, error) {
	s := ""
	keepString(&s)
	for i := 0; i < n; i++ {
		t, err := name(i)
		if err != nil {
			return s, err
		}
		s += t
	}
	return s, nil
}

func main() {
	b, err := gather(len(os.Args))
	s, err2 := joined(len(os.Args))
	println(len(b), err == nil, s, err2 == nil)
}
