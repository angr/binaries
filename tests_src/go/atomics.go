// Atomic intrinsics the arm64 compiler lowers to an LSE-or-LL/SC dispatch on runtime.arm64HasATOMICS.
package main

import (
	"fmt"
	"os"
	"sync/atomic"
)

type counter struct {
	hits  int32
	total int64
	flags uint32
	state int32
}

//go:noinline
func bump(c *counter, d int32) int32 { return atomic.AddInt32(&c.hits, d) }

//go:noinline
func bump64(c *counter, d int64) int64 { return atomic.AddInt64(&c.total, d) }

//go:noinline
func release(c *counter) int32 {
	n := atomic.AddInt32(&c.hits, -1)
	if n != 0 {
		fmt.Println("still busy", n)
	}
	return n
}

//go:noinline
func tryLock(c *counter) bool { return atomic.CompareAndSwapInt32(&c.state, 0, 1) }

//go:noinline
func lock(c *counter) {
	if atomic.CompareAndSwapInt32(&c.state, 0, 1) {
		return
	}
	for !atomic.CompareAndSwapInt32(&c.state, 0, 1) {
		fmt.Println("contended")
	}
}

//go:noinline
func unlock(c *counter) { atomic.StoreInt32(&c.state, 0) }

//go:noinline
func peek(c *counter) int32 { return atomic.LoadInt32(&c.hits) }

//go:noinline
func total(c *counter) int64 { return atomic.LoadInt64(&c.total) }

//go:noinline
func swap(c *counter, v int32) int32 { return atomic.SwapInt32(&c.state, v) }

//go:noinline
func setFlag(c *counter, bit uint32) uint32 { return atomic.OrUint32(&c.flags, bit) }

//go:noinline
func clearFlag(c *counter, bit uint32) uint32 { return atomic.AndUint32(&c.flags, ^bit) }

func main() {
	c := &counter{}
	n := int32(len(os.Args))
	fmt.Println(bump(c, n), bump64(c, int64(n)), release(c))
	fmt.Println(tryLock(c), peek(c), total(c))
	lock(c)
	unlock(c)
	fmt.Println(swap(c, n), setFlag(c, 4), clearFlag(c, 4))
}
