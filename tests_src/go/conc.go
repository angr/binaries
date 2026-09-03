// Concurrency and control idioms: goroutines, channels, select, defer, panic/recover, closures, method values and
// range loops over slices and strings.
package main

import (
	"os"
	"sync"
)

type counter struct {
	mu sync.Mutex
	n  int
}

//go:noinline
func (c *counter) inc() {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.n++
}

//go:noinline
func producer(ch chan<- int, n int) {
	for i := 0; i < n; i++ {
		ch <- i
	}
	close(ch)
}

//go:noinline
func consume(ch <-chan int) int {
	t := 0
	for v := range ch {
		t += v
	}
	return t
}

//go:noinline
func recvOne(ch <-chan int) (int, bool) {
	v, ok := <-ch
	return v, ok
}

//go:noinline
func pick(a, b <-chan int) int {
	select {
	case v := <-a:
		return v
	case v := <-b:
		return v + 100
	}
}

//go:noinline
func safeDiv(a, b int) (q int, err any) {
	defer func() {
		if r := recover(); r != nil {
			err = r
		}
	}()
	return a / b, nil
}

//go:noinline
func mustPositive(n int) int {
	if n <= 0 {
		panic("not positive")
	}
	return n
}

//go:noinline
func adder(base int) func(int) int {
	return func(x int) int {
		return base + x
	}
}

//go:noinline
func apply(f func(int) int, xs []int) []int {
	out := make([]int, 0, len(xs))
	for _, x := range xs {
		out = append(out, f(x))
	}
	return out
}

//go:noinline
func countRunes(s string) int {
	n := 0
	for range s {
		n++
	}
	return n
}

//go:noinline
func firstUpper(s string) int {
	for i, r := range s {
		if r >= 'A' && r <= 'Z' {
			return i
		}
	}
	return -1
}

//go:noinline
func runAll(fs []func()) {
	var wg sync.WaitGroup
	for _, f := range fs {
		wg.Add(1)
		go func() {
			defer wg.Done()
			f()
		}()
	}
	wg.Wait()
}

func main() {
	c := &counter{}
	ch := make(chan int, 4)
	go producer(ch, 5)
	q, err := safeDiv(7, len(os.Args)-1)
	inc := c.inc
	runAll([]func(){inc, inc, func() { c.inc() }})
	a, b := make(chan int, 1), make(chan int, 1)
	a <- 1
	v, ok := recvOne(a)
	b <- 2
	println(consume(ch), v, ok, pick(a, b), q, err == nil, mustPositive(len(os.Args)), adder(2)(3),
		len(apply(adder(1), []int{1, 2, 3})), countRunes("héllo"), firstUpper("abC"), c.n)
}
