// Small Go program exercising the constructs the Go decompiler flavor recovers first:
// integer arithmetic, recursion, multi-value returns, slices, strings, structs, globals and
// the (T, error) idiom. Every function is //go:noinline so each one keeps its own body.
package main

import (
	"errors"
	"os"
	"strconv"
)

type point struct {
	x, y int
}

var counter int

var errNegative = errors.New("negative")

//go:noinline
func add(a, b int) int {
	return a + b
}

//go:noinline
func fib(n int) int {
	if n <= 1 {
		return n
	}
	return fib(n-1) + fib(n-2)
}

//go:noinline
func divmod(a, b int) (int, int) {
	return a / b, a % b
}

//go:noinline
func sum(xs []int) int {
	s := 0
	for _, x := range xs {
		s += x
	}
	return s
}

//go:noinline
func count(s string, c byte) int {
	n := 0
	for i := 0; i < len(s); i++ {
		if s[i] == c {
			n++
		}
	}
	return n
}

//go:noinline
func parse(s string) (int, error) {
	v, err := strconv.Atoi(s)
	if err != nil {
		return 0, err
	}
	if v < 0 {
		return 0, errNegative
	}
	return v, nil
}

//go:noinline
func scale(p *point, k int) {
	p.x *= k
	p.y *= k
	counter++
}

//go:noinline
func manhattan(p point) int {
	if p.x < 0 {
		p.x = -p.x
	}
	if p.y < 0 {
		p.y = -p.y
	}
	return p.x + p.y
}

//go:noinline
func bump(xs []int) []int {
	for i := range xs {
		xs[i]++
	}
	return xs
}

func main() {
	n := 10
	if len(os.Args) > 1 {
		if v, err := parse(os.Args[1]); err == nil {
			n = v
		}
	}
	q, r := divmod(n, 3)
	p := point{x: q, y: r}
	scale(&p, 2)
	xs := bump([]int{1, 2, 3, n})
	println(add(n, 1), fib(n), q, r, sum(xs), count("hello, world", 'l'), manhattan(p), counter)
}
