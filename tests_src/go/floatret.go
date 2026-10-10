package main

import (
	"fmt"
	"os"
)

// (int, float64): RAX + X0
func ratio(a, b int) (int, float64) {
	if b == 0 {
		return 0, 0
	}
	return a / b, float64(a) / float64(b)
}

// (float64, float64): X0 + X1
func bounds(xs []float64) (float64, float64) {
	lo, hi := xs[0], xs[0]
	for _, x := range xs[1:] {
		if x < lo {
			lo = x
		}
		if x > hi {
			hi = x
		}
	}
	return lo, hi
}

// (string, float64): RAX, RBX + X0
func grade(score float64) (string, float64) {
	if score >= 90 {
		return "A", score - 90
	}
	return "B", 90 - score
}

func main() {
	n := len(os.Args)
	q, r := ratio(n*7, n+1)
	xs := []float64{float64(n), r, 3.5}
	lo, hi := bounds(xs)
	g, d := grade(hi * 10)
	fmt.Println(q, r, lo, hi, g, d)
}
