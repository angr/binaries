// Interface idioms: boxing into any/error, type assertions and switches, interface method calls, fmt varargs,
// value and pointer receivers, and the (T, error) pattern with errors.New.
package main

import (
	"errors"
	"fmt"
	"os"
	"strconv"
)

type Shape interface {
	Area() int
	Name() string
}

type Rect struct {
	w, h int
}

type Square struct {
	side int
}

func (r Rect) Area() int      { return r.w * r.h }
func (r Rect) Name() string   { return "rect" }
func (s *Square) Area() int   { return s.side * s.side }
func (s *Square) Name() string { return "square" }

var errEmpty = errors.New("empty")

//go:noinline
func describe(s Shape) string {
	return s.Name() + ":" + strconv.Itoa(s.Area())
}

//go:noinline
func box(n int) any {
	return n
}

//go:noinline
func boxString(s string) any {
	return s
}

//go:noinline
func unbox(v any) int {
	if n, ok := v.(int); ok {
		return n
	}
	return -1
}

//go:noinline
func kind(v any) string {
	switch x := v.(type) {
	case int:
		return "int:" + strconv.Itoa(x)
	case string:
		return "string:" + x
	case Shape:
		return "shape:" + x.Name()
	default:
		return "other"
	}
}

//go:noinline
func asSquare(s Shape) *Square {
	return s.(*Square)
}

//go:noinline
func first(xs []string) (string, error) {
	if len(xs) == 0 {
		return "", errEmpty
	}
	return xs[0], nil
}

//go:noinline
func report(w *os.File, shapes []Shape) {
	for i, s := range shapes {
		fmt.Fprintf(w, "%d: %s area=%d\n", i, s.Name(), s.Area())
	}
}

//go:noinline
func wrap(err error) error {
	if err != nil {
		return fmt.Errorf("wrapped: %w", err)
	}
	return nil
}

func main() {
	shapes := []Shape{Rect{2, 3}, &Square{4}}
	report(os.Stdout, shapes)
	f, err := first(os.Args)
	println(describe(shapes[0]), unbox(box(5)), kind(boxString("s")), kind(shapes[1]), asSquare(shapes[1]).side,
		f, err == nil, wrap(err) == nil)
}
