// Type switches and assertions: a hash-searched switch with a multi-type case, assertions to interfaces through
// runtime.typeAssert, a comma-ok concrete assertion whose result merges, and an interface conversion.
package main

import (
	"fmt"
	"io"
	"os"
	"strings"
)

type Event interface{ Name() string }

type MouseEvent struct {
	btn, mod int16
	x        int
}
type KeyEvent struct{ code int }
type RawEvent struct{ s string }
type KeySequenceEvent struct{ keys []KeyEvent }

func (MouseEvent) Name() string       { return "mouse" }
func (KeyEvent) Name() string         { return "key" }
func (RawEvent) Name() string         { return "raw" }
func (KeySequenceEvent) Name() string { return "seq" }

//go:noinline
func onMouse(btn, mod int16, x int) { println(btn, mod, x) }

//go:noinline
func onKey(e Event) { println(e.Name()) }

//go:noinline
func dispatch(e Event) {
	switch ev := e.(type) {
	case MouseEvent:
		onMouse(ev.btn, ev.mod, ev.x)
	case KeyEvent, KeySequenceEvent, RawEvent:
		onKey(e)
	}
}

//go:noinline
func asReader(v any) io.Reader {
	r, ok := v.(io.Reader)
	if !ok {
		return nil
	}
	return r
}

//go:noinline
func lookup(v any, keys []string) (any, bool) {
	for _, k := range keys {
		m, ok := v.(map[string]any)
		if !ok {
			return nil, false
		}
		v, ok = m[k]
		if !ok {
			return nil, false
		}
	}
	return v, true
}

//go:noinline
func toWriter(w io.ReadWriter) io.Writer {
	return w
}

func main() {
	for _, e := range []Event{MouseEvent{1, 2, 3}, KeyEvent{4}, RawEvent{"r"}, KeySequenceEvent{}} {
		dispatch(e)
	}
	fmt.Println(asReader(strings.NewReader("x")) != nil, toWriter(os.Stdout) != nil)
	fmt.Println(lookup(map[string]any{"a": map[string]any{"b": 1}}, []string{"a", "b"}))
}
