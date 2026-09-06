// Panicking type assertions on holders that are not interface-typed variables: a call result, a struct field
// read from memory, and a pointer-shaped concrete type.
package main

import "os"

type holder struct {
	v any
}

//go:noinline
func mk(n int) any {
	if n > 0 {
		return n
	}
	return "s"
}

//go:noinline
func viaCall(n int) int {
	return mk(n).(int)
}

//go:noinline
func viaField(h *holder) string {
	return h.v.(string)
}

//go:noinline
func viaFieldPtr(h *holder) *holder {
	return h.v.(*holder)
}

func main() {
	h := &holder{v: "x"}
	println(viaCall(len(os.Args)), viaField(h))
	h.v = h
	println(viaFieldPtr(h) == h)
}
