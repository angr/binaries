// A map keyed by a uint16 that each loop iteration reads through a pointer, then looks up and assigns (the inliner
// must be on). Mirrors the seenExts loop in crypto/tls's (*clientHelloMsg).unmarshal; reader's methods are a cut-down
// cryptobyte.String.
package main

import "os"

type reader []byte

func (s *reader) read(n int) []byte {
	if len(*s) < n || n < 0 {
		return nil
	}
	v := (*s)[:n:n]
	*s = (*s)[n:]
	return v
}

func (s *reader) readUint16(out *uint16) bool {
	v := s.read(2)
	if v == nil {
		return false
	}
	*out = uint16(v[0])<<8 | uint16(v[1])
	return true
}

func (s *reader) readPrefixed(out *reader) bool {
	var n uint16
	if !s.readUint16(&n) {
		return false
	}
	v := s.read(int(n))
	if v == nil {
		return false
	}
	*out = v
	return true
}

func (s *reader) empty() bool {
	return len(*s) == 0
}

// distinctTags reads (tag, length-prefixed body) records and refuses a repeated tag.
//
//go:noinline
func distinctTags(data []byte) ([]uint16, bool) {
	s := reader(data)
	seen := make(map[uint16]bool)
	var tags []uint16
	for !s.empty() {
		var tag uint16
		var body reader
		if !s.readUint16(&tag) || !s.readPrefixed(&body) {
			return nil, false
		}
		if seen[tag] {
			return nil, false
		}
		seen[tag] = true
		tags = append(tags, tag)
	}
	return tags, true
}

func main() {
	tags, ok := distinctTags([]byte(os.Args[0]))
	println(len(tags), ok)
}
