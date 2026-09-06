// A method with a struct value receiver holding a string; on 386 the receiver spans two stack words.
package main

import "os"

type gitHubRecipientError struct {
	username string
}

//go:noinline
func (e gitHubRecipientError) Error() string {
	return "github recipient " + e.username + " has no public keys"
}

func main() {
	err := gitHubRecipientError{username: os.Args[0]}
	os.Exit(len(err.Error()))
}
