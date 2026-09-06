// A 4-byte struct field read on a path where the only write to it is a wider copy: the struct is built in a
// temporary and copied over with 16-byte moves (the inliner must be on). Mirrors delve's
// ebpf.(*EBPFContext).handleHeaderEvent; handleHeader is the smallest shape that still shows it.
package main

import (
	"encoding/binary"
	"os"
	"sync"
)

type eventHeader struct {
	Type        uint8
	GoroutineID int64
	FnAddr      uint64
	IsRet       bool
	NParams     uint32
}

type pendingKey struct {
	goroutineID int64
	fnAddr      uint64
	isRet       bool
}

type pendingEvent struct {
	header eventHeader
	params []*int
}

type context struct {
	m       sync.Mutex
	pending map[pendingKey]*pendingEvent
	parsed  []pendingKey
	last    *pendingEvent
}

func parseEventHeader(b []byte) (eventHeader, bool) {
	if len(b) < 22 {
		return eventHeader{}, false
	}
	var h eventHeader
	h.Type = b[0]
	h.GoroutineID = int64(binary.LittleEndian.Uint64(b[1:9]))
	h.FnAddr = binary.LittleEndian.Uint64(b[9:17])
	h.IsRet = b[17] != 0
	h.NParams = binary.LittleEndian.Uint32(b[18:22])
	return h, true
}

//go:noinline
func (ctx *context) emit(pe *pendingEvent) {
	ctx.parsed = append(ctx.parsed, pendingKey{pe.header.GoroutineID, pe.header.FnAddr, pe.header.IsRet})
}

//go:noinline
func (ctx *context) handleHeader(raw []byte) int {
	hdr, ok := parseEventHeader(raw)
	if !ok {
		return -1
	}
	ctx.m.Lock()
	defer ctx.m.Unlock()
	if hdr.NParams == 0 {
		return 0
	}
	if hdr.NParams > 6 {
		return 1
	}
	ctx.last = &pendingEvent{header: hdr, params: make([]*int, hdr.NParams)}
	return 2
}

//go:noinline
func (ctx *context) handleHeaderEvent(raw []byte) {
	hdr, ok := parseEventHeader(raw)
	if !ok {
		return
	}
	key := pendingKey{goroutineID: hdr.GoroutineID, fnAddr: hdr.FnAddr, isRet: hdr.IsRet}
	ctx.m.Lock()
	defer ctx.m.Unlock()
	if old, exists := ctx.pending[key]; exists {
		ctx.emit(old)
		delete(ctx.pending, key)
	}
	if len(ctx.pending) >= 1000 {
		for k, pe := range ctx.pending {
			ctx.emit(pe)
			delete(ctx.pending, k)
		}
	}
	if hdr.NParams == 0 {
		ctx.parsed = append(ctx.parsed, key)
	} else {
		if hdr.NParams > 6 {
			return
		}
		ctx.pending[key] = &pendingEvent{header: hdr, params: make([]*int, hdr.NParams)}
	}
}

func main() {
	ctx := &context{pending: map[pendingKey]*pendingEvent{}}
	raw := make([]byte, 22)
	for i := range raw {
		raw[i] = byte(len(os.Args) + i)
	}
	ctx.handleHeaderEvent(raw)
	os.Exit(ctx.handleHeader(raw) + len(ctx.parsed))
}
