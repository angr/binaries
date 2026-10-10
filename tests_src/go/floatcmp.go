package main

import (
	"fmt"
	"os"
	"strconv"
)

// float compares against literals in branches the string-compare folder inspects
func stage(progress float64) string {
	if progress > 0.5 {
		return "late"
	}
	if progress == 1.0 {
		return "done"
	}
	return "early"
}

// float selects between literals, live across a call (ITEs with float arms)
func perSecond(n int64, state int, status int) float64 {
	synced := 1.0
	if state == 5 {
		synced = 0
	}
	divisor := 1_000_000.0
	if status&0x2000 == 0x2000 {
		divisor = 1_000_000_000.0
	}
	m := tick(n)
	if m > 10 {
		return float64(m) / divisor
	}
	return synced * float64(m)
}

func tick(n int64) int64 {
	fmt.Println(n)
	return n + 1
}

func main() {
	v, _ := strconv.ParseFloat(os.Args[len(os.Args)-1], 64)
	rate := perSecond(int64(len(os.Args)), len(os.Args[0]), int(v))
	fmt.Println(stage(v), rate)
}
