// String and slice values the inliner leaves behind: strings.Split/SplitAfter with literal separators inline into
// genSplit calls, and map assignments whose value is a string or slice field read through a pointer.
package main

import (
	"os"
	"strings"
)

type rec struct {
	id    int
	name  string
	items []int
}

//go:noinline
func fields(s string) []string {
	return strings.SplitAfter(s, ",")
}

//go:noinline
func fieldsN(s string) []string {
	return strings.SplitAfterN(s, "::", 3)
}

//go:noinline
func parts(s string) []string {
	return strings.Split(s, ";")
}

//go:noinline
func byName(m map[int]string, r *rec) {
	m[r.id] = r.name
}

//go:noinline
func byItems(m map[int][]int, r *rec) {
	m[r.id] = r.items
}

func main() {
	r := &rec{1, os.Args[0], []int{1}}
	names := map[int]string{}
	byName(names, r)
	items := map[int][]int{}
	byItems(items, r)
	println(len(fields(os.Args[0])), len(fieldsN(os.Args[0])), len(parts(os.Args[0])), len(names), len(items))
}
