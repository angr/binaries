// A lexer driven by state functions, the text/template/parse shape: each state takes the lexer and returns the next
// state, and the driver loop calls whatever func value the previous state returned.
package main

import "os"

type lexer struct {
	input        string
	pos          int
	insideAction bool
	words        int
}

type stateFn func(*lexer) stateFn

//go:noinline
func lexText(l *lexer) stateFn {
	for l.pos < len(l.input) {
		if l.input[l.pos] == '{' {
			l.pos++
			return lexInsideAction
		}
		l.pos++
	}
	return nil
}

//go:noinline
func lexInsideAction(l *lexer) stateFn {
	for l.pos < len(l.input) {
		if l.input[l.pos] == '}' {
			l.pos++
			l.words++
			return lexText
		}
		l.pos++
	}
	return nil
}

//go:noinline
func (l *lexer) run() int {
	state := lexText
	if l.insideAction {
		state = lexInsideAction
	}
	for {
		state = state(l)
		if state == nil {
			return l.words
		}
	}
}

func main() {
	l := &lexer{input: os.Args[0], insideAction: len(os.Args) > 1}
	println(l.run())
}
