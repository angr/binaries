// Map idioms whose keys or values span several words: struct keys, slice values, empty-struct values, and maps
// created into struct fields.
package main

import "os"

type key struct {
	kind int
	name string
}

type store struct {
	byKey  map[key][]int
	flags  map[string]struct{}
	counts map[key]int
}

//go:noinline
func newStore() *store {
	return &store{byKey: map[key][]int{}, flags: map[string]struct{}{}, counts: map[key]int{}}
}

//go:noinline
func put(s *store, k key, v []int) {
	s.byKey[k] = v
}

//go:noinline
func get(s *store, k key) ([]int, bool) {
	v, ok := s.byKey[k]
	return v, ok
}

//go:noinline
func flag(s *store, name string) {
	s.flags[name] = struct{}{}
}

//go:noinline
func bump(s *store, k key) {
	s.counts[k]++
}

func main() {
	s := newStore()
	k := key{1, os.Args[0]}
	put(s, k, []int{1, 2})
	v, ok := get(s, k)
	flag(s, "x")
	bump(s, k)
	println(len(v), ok, len(s.flags), s.counts[k])
}
