// A loop serving requests from a channel of structs: for r := range reqs with map updates, a func value call and
// close. On 32-bit arm the receive's ok flag lands in a stack word that other calls also pass arguments in.
package main

import (
	"io"
	"os"
)

type request struct {
	op   string
	id   int
	msg  string
	w    io.Writer
	done chan struct{}
}

var (
	reqs = make(chan request)
	quit = make(chan struct{}, 1)
	mix  = weight
)

//go:noinline
func weight(a, b int) int { return a*31 + b }

//go:noinline
func serve() int {
	writers := make(map[int]io.Writer)
	n := 0
	for r := range reqs {
		n = mix(n, r.id)
		switch r.op {
		case "add":
			writers[r.id] = r.w
		case "drop":
			delete(writers, r.id)
		case "quit":
			quit <- struct{}{}
			close(r.done)
			return n
		}
		if r.done != nil {
			close(r.done)
		}
	}
	return n
}

func main() {
	go serve()
	done := make(chan struct{})
	reqs <- request{op: "add", id: 1, w: os.Stdout}
	reqs <- request{op: "drop", id: 1, msg: os.Args[0]}
	reqs <- request{op: "quit", done: done}
	<-done
	<-quit
}
