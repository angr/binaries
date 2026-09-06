package main

import "os"

type pair struct{ a, b int }

type node struct {
	p *pair
	n int
}

// Parallel assignment through a slice: the loaded element must survive the first store.

//go:noinline
func swapInts(s []int, i, j int) {
	s[i], s[j] = s[j], s[i]
}

//go:noinline
func swapPairs(s []pair, i, j int) {
	s[i], s[j] = s[j], s[i]
}

//go:noinline
func swapNodes(s []node, i, j int) {
	s[i], s[j] = s[j], s[i]
}

func main() {
	s := []int{1, 2, 3}
	p := []pair{{1, 2}, {3, 4}}
	n := []node{{&p[0], 1}, {&p[1], 2}}
	swapInts(s, 0, len(os.Args)%3)
	swapPairs(p, 0, len(os.Args)%2)
	swapNodes(n, 0, len(os.Args)%2)
	println(s[0], p[0].a, n[0].n)
}
