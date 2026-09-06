// Comparisons and copies the runtime implements: interface equality, typed slice copies, byte arrays to strings,
// and slices that leave a stack buffer.
package main

import "os"

type T struct{ n int }

type Shape interface{ Area() int }

func (t *T) Area() int { return t.n }

//go:noinline
func same(a, b Shape) bool {
	return a == b
}

//go:noinline
func clonePtrs(s []*T) []*T {
	out := make([]*T, len(s))
	copy(out, s)
	return out
}

//go:noinline
func hexOf(b byte) string {
	var buf [2]byte
	buf[0] = "0123456789abcdef"[b>>4]
	buf[1] = "0123456789abcdef"[b&15]
	return string(buf[:])
}

//go:noinline
func digits(n int) []byte {
	out := make([]byte, 0, n%8+1)
	for n > 0 {
		out = append(out, byte('0'+n%10))
		n /= 10
	}
	return out
}

func main() {
	t := &T{1}
	ptrs := clonePtrs([]*T{t, t})
	println(same(t, ptrs[0]), same(t, nil), hexOf(byte(len(os.Args))), string(digits(len(os.Args)*7)))
}
