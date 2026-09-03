// Runtime-call idioms the Go decompiler flavor rewrites into builtins: allocation, slices, strings, bounds and
// nil checks, and write barriers.
package main

import "os"

type node struct {
	val  int
	next *node
}

var head *node

//go:noinline
func newNode(v int) *node {
	return &node{val: v}
}

//go:noinline
func push(v int) {
	n := newNode(v)
	n.next = head
	head = n
}

//go:noinline
func mkslice(n int) []int {
	s := make([]int, n)
	for i := range s {
		s[i] = i * i
	}
	return s
}

//go:noinline
func appendOne(s []int, v int) []int {
	return append(s, v)
}

//go:noinline
func third(s []int) int {
	return s[2]
}

//go:noinline
func at(s []int, i int) int {
	return s[i]
}

//go:noinline
func tail(s []int) []int {
	return s[1:]
}

//go:noinline
func window(s []int, i, j int) []int {
	return s[i:j]
}

//go:noinline
func concat(a, b string) string {
	return a + b
}

//go:noinline
func concat3(a, b, c string) string {
	return a + b + c
}

//go:noinline
func equal(a, b string) bool {
	return a == b
}

//go:noinline
func less(a, b string) bool {
	return a < b
}

//go:noinline
func toBytes(s string) []byte {
	return []byte(s)
}

//go:noinline
func toString(b []byte) string {
	return string(b)
}

//go:noinline
func copyInts(dst, src []int) int {
	return copy(dst, src)
}

//go:noinline
func deref(p *node) int {
	return p.val
}

//go:noinline
func stringLen(s string) int {
	return len(s)
}

func main() {
	for i := 0; i < 3; i++ {
		push(i)
	}
	s := mkslice(len(os.Args) + 4)
	s = appendOne(s, 7)
	dst := make([]int, 2)
	println(third(s), at(s, 1), len(tail(s)), len(window(s, 1, 3)), copyInts(dst, s))
	name := concat("go", "pher")
	println(name, concat3("a", name, "z"), equal(name, os.Args[0]), less("a", name), len(toBytes(name)),
		toString([]byte{104, 105}), deref(head), stringLen(name))
}
