package main

import (
	"fmt"
	"math"
	"os"
	"strconv"
)

// x == c branches to the same block on ZF=0 and on PF=1 (unordered)
func eqConst(x float64) int {
	if x == 1.5 {
		return 1
	}
	return 2
}

func neConst(x float64) int {
	if x != 2.5 {
		return 3
	}
	return 4
}

func eqVars(x, y float64) int {
	if x == y {
		return 7
	}
	return 9
}

// self-compares are NaN tests
func isNaN(x float64) bool {
	return x != x
}

func isNaN32(x float32) bool {
	return x != x
}

func root(x float64) float64 {
	return math.Sqrt(x)
}

// float zeroed in one branch (xorps)
func clamp(x float64) float64 {
	if x < 0 {
		x = 0
	}
	return x * 3
}

func zeroIfEq(x, y float64) float64 {
	r := y
	if x == 4.0 {
		r = 0
	}
	return r + 1
}

func main() {
	v, _ := strconv.ParseFloat(os.Args[len(os.Args)-1], 64)
	w, _ := strconv.ParseFloat(os.Args[0], 64)
	fmt.Println(eqConst(v), neConst(v), eqVars(v, w), isNaN(v), isNaN32(float32(w)), root(v), clamp(v), zeroIfEq(v, w))
}
