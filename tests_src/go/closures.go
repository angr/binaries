// Closures and package-level variables: capture-less func literals (static funcvals), escaping closures with
// captures (heap closure records), a stack closure passed to a non-escaping parameter, calls through func values,
// and package variables initialized at run time (typed only by their initializers).
package main

import (
	"errors"
	"os"
	"sort"
	"strings"
)

type item struct {
	name  string
	score int
}

var (
	errEmpty = errors.New("empty")
	names    = strings.Split("carol,alice,bob", ",")
	weights  = map[string]int{"alice": 3, "bob": 1}
	sink     *item
)

//go:noinline
func firstUpper(s string) int {
	return strings.IndexFunc(s, func(r rune) bool { return r >= 'A' && r <= 'Z' })
}

//go:noinline
func scaler(k int) func(int) int {
	return func(x int) int { return x * k }
}

//go:noinline
func applyAll(f func(int) int, xs []int) int {
	t := 0
	for _, x := range xs {
		t += f(x)
	}
	return t
}

//go:noinline
func sortItems(items []item) {
	sort.Slice(items, func(i, j int) bool { return items[i].score > items[j].score })
}

//go:noinline
func counter() (func() int, func()) {
	n := 0
	inc := func() int { n++; return n }
	reset := func() { n = 0 }
	return inc, reset
}

//go:noinline
func pick(items []item, name string) (*item, error) {
	if len(items) == 0 {
		return nil, errEmpty
	}
	for i := range items {
		if items[i].name == name {
			sink = &items[i]
			return sink, nil
		}
	}
	return nil, errors.New("missing " + name)
}

func main() {
	items := []item{{"alice", 2}, {"bob", 5}, {"carol", 1}}
	sortItems(items)
	inc, reset := counter()
	inc()
	inc()
	if len(os.Args) > 1 {
		reset()
	}
	p, err := pick(items, names[0])
	done := make(chan int)
	go func() { done <- applyAll(scaler(inc()), []int{1, 2, 3}) }()
	println(firstUpper("abC"), <-done, err == nil, p != nil, weights["alice"], len(names))
}
