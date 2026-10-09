// Map creation shapes: non-escaping maps the compiler builds on the stack (a literal with a zero-size value type,
// a make with a variable hint), and makemap_small results whose type only shows where the map is put (boxed in an
// interface, returned as a named map type, stored into a struct field).
package main

import "os"

type set map[string]struct{}

type registry struct {
	name  string
	items map[string]int
}

//go:noinline
func uniq(xs []string) int {
	seen := map[string]struct{}{}
	n := 0
	for _, x := range xs {
		if _, ok := seen[x]; !ok {
			seen[x] = struct{}{}
			n++
		}
	}
	return n
}

//go:noinline
func counts(xs []string) int {
	m := make(map[string]int, len(xs))
	for _, x := range xs {
		m[x]++
	}
	return len(m)
}

//go:noinline
func boxed() any {
	return make(map[string]bool)
}

//go:noinline
func newSet() set {
	return make(set)
}

//go:noinline
func newRegistry(name string) *registry {
	r := &registry{name: name}
	r.items = make(map[string]int)
	return r
}

func main() {
	args := os.Args
	r := newRegistry(args[0])
	r.items[args[0]] = 1
	println(uniq(args), counts(args), boxed() != nil, len(newSet()), len(r.items))
}
