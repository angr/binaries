// container/list calls the inliner flattens into the caller (the inliner must be on): list.New, PushBack and
// PushFront (lazyInit + insert), Front/Back and Element.Next/Prev.
package main

import (
	"container/list"
	"os"
)

var sink any

// List-shaped: n at the offset of List.len, head at the offset of List.root.next.
type ring struct {
	head *ring
	_    [4]int
	n    int
}

func ringHead(r *ring) *ring {
	if r.n == 0 {
		return nil
	}
	return r.head
}

//go:noinline
func newList() *list.List {
	return list.New()
}

//go:noinline
func pushBack(l *list.List, v int) *list.Element {
	return l.PushBack(v)
}

//go:noinline
func pushFront(l *list.List, s string) {
	l.PushFront(s)
}

//go:noinline
func front(l *list.List) *list.Element {
	return l.Front()
}

//go:noinline
func back(l *list.List) *list.Element {
	return l.Back()
}

//go:noinline
func nextValue(e *list.Element) any {
	n := e.Next()
	if n == nil {
		return nil
	}
	return n.Value
}

//go:noinline
func prevOf(e *list.Element, out **list.Element) {
	*out = e.Prev()
}

//go:noinline
func sum(l *list.List) int {
	t := 0
	for e := l.Front(); e != nil; e = e.Next() {
		t += e.Value.(int)
	}
	return t
}

//go:noinline
func newFront() *list.Element {
	l := list.New()
	sink = l
	return l.Front()
}

//go:noinline
func newRing(n int) *ring {
	r := new(ring)
	r.n = n
	sink = r
	return ringHead(r)
}

func main() {
	l := newList()
	for i := range len(os.Args) {
		pushBack(l, i)
	}
	pushFront(l, os.Args[0])
	var p *list.Element
	prevOf(back(l), &p)
	if nextValue(front(l)) == nil || p == nil || newFront() != nil || newRing(1) != nil {
		os.Exit(1)
	}
	os.Exit(sum(newList()))
}
