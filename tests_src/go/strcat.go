package main

import (
	"fmt"
	"os"
)

// h += <call result>: on 386 the old h and the call result share the result slot of the frame
func usage() string {
	h := "Usage:\n"
	h += "  strcat [OPTIONS]\n\n"
	h += "Exit Codes:\n"
	h += fmt.Sprintf("  %d\t%s\n", 0, "OK")
	h += fmt.Sprintf("  %d\t%s\n", 1, "Failed")
	h += "\n"
	return h
}

func main() {
	fmt.Fprint(os.Stderr, usage())
}
