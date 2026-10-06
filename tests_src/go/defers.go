// Defers the compiler cannot open-code: a defer in a loop goes through runtime.deferproc and a function
// with more than eight defers through runtime.deferprocStack. Up to go1.22 the call site tests the
// status deferproc leaves in AX and branches to the shared deferreturn exit.
package main

import "fmt"

var released []int

//go:noinline
func release(i int) { released = append(released, i) }

//go:noinline
func releaseAll(n int) int {
	for i := 0; i < n; i++ {
		defer release(i)
	}
	return len(released)
}

//go:noinline
func nineDefers(x int) int {
	defer release(1)
	defer release(2)
	defer release(3)
	defer release(4)
	defer release(5)
	defer release(6)
	defer release(7)
	defer release(8)
	defer release(9)
	return x + len(released)
}

func main() {
	fmt.Println(releaseAll(3), nineDefers(4), released)
}
