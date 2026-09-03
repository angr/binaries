// Map idioms: make, lookup with and without the ok flag, assignment, delete, len and range.
package main

import "os"

var table = map[string]int{"one": 1, "two": 2}

//go:noinline
func lookup(m map[string]int, k string) int {
	return m[k]
}

//go:noinline
func lookupOk(m map[string]int, k string) (int, bool) {
	v, ok := m[k]
	return v, ok
}

//go:noinline
func store(m map[string]int, k string, v int) {
	m[k] = v
}

//go:noinline
func remove(m map[string]int, k string) {
	delete(m, k)
}

//go:noinline
func total(m map[string]int) int {
	t := 0
	for _, v := range m {
		t += v
	}
	return t
}

//go:noinline
func keys(m map[string]int) []string {
	out := make([]string, 0, len(m))
	for k := range m {
		out = append(out, k)
	}
	return out
}

//go:noinline
func counts(words []string) map[string]int {
	m := make(map[string]int)
	for _, w := range words {
		m[w]++
	}
	return m
}

//go:noinline
func byInt(m map[int]string, k int) string {
	return m[k]
}

func main() {
	store(table, "three", 3)
	remove(table, "one")
	v, ok := lookupOk(table, os.Args[0])
	println(lookup(table, "two"), v, ok, total(table), len(keys(table)), len(counts(os.Args)),
		byInt(map[int]string{1: "x"}, 1))
}
