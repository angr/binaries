// Slice growth idioms: append on a struct field, on a loop-carried local, a variadic append, and array-backed slices.
package main

import "os"

type bag struct {
	items []int
	names []string
}

//go:noinline
func (b *bag) add(v int) {
	b.items = append(b.items, v)
}

//go:noinline
func (b *bag) addName(s string) {
	b.names = append(b.names, s)
}

//go:noinline
func squares(n int) []int {
	var out []int
	for i := 0; i < n; i++ {
		out = append(out, i*i)
	}
	return out
}

//go:noinline
func concat(a, b []int) []int {
	return append(a, b...)
}

//go:noinline
func withPrefix(s []byte) []byte {
	buf := [8]byte{'>', ' '}
	return append(buf[:2], s...)
}

//go:noinline
func pair(a, b string) []string {
	return append([]string{a}, b)
}

func main() {
	b := &bag{}
	for i := range os.Args {
		b.add(i)
		b.addName(os.Args[i])
	}
	println(len(b.items), len(b.names), len(squares(len(os.Args))), len(concat(b.items, b.items)),
		string(withPrefix([]byte(os.Args[0]))), len(pair("a", "b")))
}
