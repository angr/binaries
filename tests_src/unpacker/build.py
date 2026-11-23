#!/usr/bin/env python3
"""
Build the hand-made PE files of angr's unpacker tests (tests/analyses/unpacker) into tests/i386/windows/unpacker and
tests/x86_64/windows/unpacker.

Usage: build.py [output root, default: the tests directory next to tests_src]
"""

from __future__ import annotations

import lzma
import os
import struct
import sys

SECTION_ALIGNMENT = 0x1000
FILE_ALIGNMENT = 0x200
IMAGE_BASE = 0x400000
IMAGE_BASE_64 = 0x140000000
CODE_RVA = 0x1000
BASE = IMAGE_BASE + CODE_RVA

Section = tuple[bytes, int, int, bytes, int]


def p32(value: int) -> bytes:
    return struct.pack("<I", value)


def build_pe(
    sections: list[Section],
    entry_rva: int,
    is_64bit: bool = False,
    directories: dict[int, tuple[int, int]] | None = None,
) -> bytes:
    """
    Build a minimal PE file. Each section is (name, rva, virtual size, raw data, characteristics); directories maps
    a data-directory index to (rva, size).
    """
    headers_size = 0x400
    dos = bytearray(0x80)
    dos[0:2] = b"MZ"
    struct.pack_into("<I", dos, 0x3C, 0x80)
    image_size = max(
        rva + ((vsize + SECTION_ALIGNMENT - 1) & ~(SECTION_ALIGNMENT - 1)) for _, rva, vsize, _, _ in sections
    )
    if is_64bit:
        file_header = struct.pack("<HHIIIHH", 0x8664, len(sections), 0, 0, 0, 0xF0, 0x22)
        optional = struct.pack(
            "<HBBIIIIIQIIHHHHHHIIIIHHQQQQII",
            0x20B, 14, 0, 0, 0, 0, entry_rva, 0x1000,
            IMAGE_BASE_64, SECTION_ALIGNMENT, FILE_ALIGNMENT, 6, 0, 0, 0, 6, 0, 0, image_size, headers_size, 0, 3, 0,
            0x100000, 0x1000, 0x100000, 0x1000, 0, 16,
        )  # fmt: skip
    else:
        file_header = struct.pack("<HHIIIHH", 0x14C, len(sections), 0, 0, 0, 0xE0, 0x102)
        optional = struct.pack(
            "<HBBIIIIIIIIIHHHHHHIIIIHHIIIIII",
            0x10B, 14, 0, 0, 0, 0, entry_rva, 0x1000, 0x1000,
            IMAGE_BASE, SECTION_ALIGNMENT, FILE_ALIGNMENT, 6, 0, 0, 0, 6, 0, 0, image_size, headers_size, 0, 3, 0,
            0x100000, 0x1000, 0x100000, 0x1000, 0, 16,
        )  # fmt: skip
    data_dirs = bytearray(8 * 16)
    for index, (rva, size) in (directories or {}).items():
        struct.pack_into("<II", data_dirs, 8 * index, rva, size)
    table = bytearray()
    body = bytearray()
    offset = headers_size
    for name, rva, vsize, raw, characteristics in sections:
        raw_size = (len(raw) + FILE_ALIGNMENT - 1) & ~(FILE_ALIGNMENT - 1)
        table += struct.pack(
            "<8sIIIIIIHHI", name, vsize, rva, raw_size, offset if raw else 0, 0, 0, 0, 0, characteristics
        )
        body += raw.ljust(raw_size, b"\0")
        offset += raw_size
    header = bytes(dos) + b"PE\0\0" + file_header + optional + bytes(data_dirs) + bytes(table)
    return header.ljust(headers_size, b"\0") + bytes(body)


def fake_code(size: int, rva: int, functions: int = 30) -> bytes:
    """
    Bytes that look like compiler output to call-structure heuristics: every function is called three times.
    """
    code = bytearray(b"\x90" * size)
    targets = [rva + size - 0x10 * (i + 1) for i in range(functions)]
    pos = 0
    for target in targets:
        for _ in range(3):
            code[pos : pos + 5] = b"\xe8" + struct.pack("<i", target - (rva + pos + 5))
            pos += 8
    for target in targets:
        code[target - rva] = 0xC3
    return bytes(code)


def host(code: bytes, is_64bit: bool = False, directories: dict[int, tuple[int, int]] | None = None) -> bytes:
    """
    One writable, executable .text section of a page that holds the code; the entry point is its first byte.
    """
    sections = [(b".text", CODE_RVA, 0x1000, code, 0xE0000020)]
    return build_pe(sections, entry_rva=CODE_RVA, is_64bit=is_64bit, directories=directories)


def at(size: int, *parts: tuple[int, bytes]) -> bytes:
    code = bytearray(size)
    for offset, data in parts:
        code[offset : offset + len(data)] = data
    return bytes(code)


# Threads and timers: the import cells at +0x200 are filled in by the test


def sleep_poll_long_sleeper() -> bytes:
    # main: CreateThread(worker); 3 x Sleep(50); cell[1] = GetTickCount(); Sleep(60000); ret
    # worker: Sleep(60000); cell[0] = 1; ret 4
    iat, cell, worker = BASE + 0x200, BASE + 0x300, BASE + 0x80
    call = lambda idx: b"\xff\x15" + p32(iat + 4 * idx)
    sleep_60s = b"\x68" + p32(60000) + call(0)
    main = (
        b"\x6a\x00\x6a\x00\x6a\x00\x68" + p32(worker) + b"\x6a\x00\x6a\x00" + call(1)
        + b"\xbe" + p32(3) + b"\x6a\x32" + call(0) + b"\x4e\x75\xf5"
        + call(2) + b"\xa3" + p32(cell + 4) + sleep_60s + b"\xc3"
    )  # fmt: skip
    body = sleep_60s + b"\xc7\x05" + p32(cell) + p32(1) + b"\xc2\x04\x00"
    return host(at(0x400, (0, main), (0x80, body)))


def sleep_poll_loop() -> bytes:
    # main: CreateThread(worker); while (!flag) Sleep(50); ret        worker: Sleep(60000); flag = 1; ret 4
    iat, flag, worker = BASE + 0x200, BASE + 0x300, BASE + 0x80
    call = lambda idx: b"\xff\x15" + p32(iat + 4 * idx)
    loop = b"\x6a\x32" + call(0) + b"\x83\x3d" + p32(flag) + b"\x00"
    main = b"\x6a\x00\x6a\x00\x6a\x00\x68" + p32(worker) + b"\x6a\x00\x6a\x00" + call(1)
    main += loop + b"\x74" + bytes([0x100 - len(loop) - 2]) + b"\xc3"
    body = b"\x68" + p32(60000) + call(0) + b"\xc7\x05" + p32(flag) + p32(1) + b"\xc2\x04\x00"
    return host(at(0x400, (0, main), (0x80, body)))


def shared_data_time() -> bytes:
    # 2 x GetTickCount, then copy KUSER_SHARED_DATA's TickCount, InterruptTime and SystemTime (low parts)
    iat, cell = BASE + 0x200, BASE + 0x300
    main = (b"\xff\x15" + p32(iat)) * 2
    for idx, offset in enumerate((0x320, 0x08, 0x14, 0x04)):
        main += b"\xa1" + p32(0x7FFE0000 + offset) + b"\xa3" + p32(cell + 4 * idx)
    return host(main + b"\xc3")


# COM


def guid(text: str) -> bytes:
    first, second, third, fourth, fifth = text.split("-")
    return struct.pack("<IHH", int(first, 16), int(second, 16), int(third, 16)) + bytes.fromhex(fourth + fifth)


class ComProgram:
    """
    x86 guest code that calls APIs and COM methods, storing eax and esp after each call: call n writes them to
    RESULTS + 8n. The test fills the import cells at IAT in the order of `apis`.
    """

    IAT, RESULTS, DATA = BASE + 0x600, BASE + 0x700, BASE + 0x900

    def __init__(self):
        self.code = b""
        self.data = b""
        self.apis: list[tuple[str, str]] = []
        self.calls = 0

    def put(self, data: bytes) -> int:
        addr = self.DATA + len(self.data)
        self.data += data + bytes(-len(data) % 4)
        return addr

    def _record(self) -> int:
        # mov [results + 8n], eax; mov [results + 8n + 4], esp
        self.code += b"\xa3" + p32(self.RESULTS + 8 * self.calls) + b"\x89\x25" + p32(self.RESULTS + 8 * self.calls + 4)
        self.calls += 1
        return self.calls - 1

    def _push(self, args: tuple[int, ...]) -> None:
        for arg in reversed(args):
            self.code += b"\x68" + p32(arg)

    def api(self, dll: str, name: str, *args: int) -> int:
        if (dll, name) not in self.apis:
            self.apis.append((dll, name))
        self._push(args)
        self.code += b"\xff\x15" + p32(self.IAT + 4 * self.apis.index((dll, name)))
        return self._record()

    def method(self, cell: int, index: int, *args: int) -> int:
        # mov eax, [cell]; push eax; mov ecx, [eax]; call [ecx + 4 * index]
        self._push(args)
        self.code += b"\xa1" + p32(cell) + b"\x50\x8b\x08\xff\x91" + p32(4 * index)
        return self._record()

    def build(self) -> bytes:
        assert len(self.code) < 0x600 and len(self.data) < 0x700
        return host(at(0x1000, (0, self.code + b"\xc3"), (0x900, self.data)))


def com_shell_link() -> bytes:
    shell_link = "00021401-0000-0000-c000-000000000046"
    iid_shell_link_a = "000214ee-0000-0000-c000-000000000046"
    iid_persist_file = "0000010b-0000-0000-c000-000000000046"
    prog = ComProgram()
    clsid, iid_link, iid_file = (prog.put(guid(g)) for g in (shell_link, iid_shell_link_a, iid_persist_file))
    iid_unknown = prog.put(guid("00000000-0000-0000-c000-000000000046"))
    other = prog.put(guid("12345678-9abc-def0-1122-334455667788"))
    link, file, unknown, none = (prog.put(b"\xee" * 4) for _ in range(4))
    text = prog.put(b"C:\\x.lnk\0")
    prog.api("ole32.dll", "CoInitialize", 0)
    prog.api("ole32.dll", "CoCreateInstance", clsid, 0, 1, iid_link, link)
    prog.method(link, 0, iid_file, file)
    prog.method(file, 6, text, 1)  # IPersistFile::Save
    prog.method(link, 20, text)  # IShellLinkA::SetPath
    prog.method(link, 19, 0, 0)  # IShellLinkA::Resolve
    prog.method(file, 0, iid_unknown, unknown)
    prog.method(link, 0, other, none)
    prog.method(link, 1)
    for cell in (file, unknown, link, link):
        prog.method(cell, 2)
    prog.api("ole32.dll", "CoCreateInstance", other, 0, 1, other, none)
    prog.api("ole32.dll", "CoCreateInstance", clsid, 0, 1, other, none)
    return prog.build()


def com_allocator_stream_wmi() -> bytes:
    prog = ComProgram()
    malloc, stream, locator, services, block = (prog.put(b"\xee" * 4) for _ in range(5))
    buf, count = prog.put(b"hello" + bytes(11)), prog.put(bytes(4))
    wbem = prog.put(guid("4590f811-1d3a-11d0-891f-00aa004b2e24"))
    iid_locator = prog.put(guid("dc12a687-737f-11cf-884d-00aa004b2e24"))
    prog.api("shell32.dll", "SHGetMalloc", malloc)
    prog.method(malloc, 3, 0x20)  # Alloc
    prog.code += b"\xa3" + p32(block)
    prog.method(malloc, 6, 0)  # GetSize(NULL)
    prog.api("ole32.dll", "CoTaskMemAlloc", 0x10)
    prog.api("ole32.dll", "CoTaskMemFree", 0)
    prog.api("ole32.dll", "CreateStreamOnHGlobal", 0, 1, stream)
    prog.method(stream, 4, buf, 5, 0)  # Write
    prog.method(stream, 5, 1, 0, 0, 0)  # Seek(1, STREAM_SEEK_SET, NULL): a 64-bit argument
    prog.method(stream, 3, buf + 8, 8, count)  # Read
    prog.method(stream, 10, 0, 0, 1, 0, 1)  # LockRegion: two 64-bit arguments
    prog.api("ole32.dll", "CoCreateInstance", wbem, 0, 1, iid_locator, locator)
    prog.method(locator, 3, 0, 0, 0, 0, 0, 0, 0, services)  # ConnectServer
    return prog.build()


# Window messages: import cells at +0x800, WNDCLASSEXA at +0x900, the class name at +0xA00 (filled in by the test)

# RegisterClassExA, CreateWindowExA(style), then GetMessageA/DispatchMessageA until WM_QUIT; returns its wParam
WINDOW_MAIN = (
    "6800194000ff15001840006a0068000040006a006a006a646a646a006a0068{style}6a0068001a40006a08ff1504184000"
    "a3041c40006a006a006a0068001b4000ff150818400085c0740d68001b4000ff150c184000ebdea1081b4000c3"
)


def window(style: int, wndproc: bytes) -> bytes:
    main = bytes.fromhex(WINDOW_MAIN.format(style=p32(style).hex()))
    return host(at(0x1000, (0, main), (0x200, wndproc)))


def window_timer_and_destroy() -> bytes:
    # a hidden window: the procedure sets a timer on WM_CREATE, destroys the window on the third WM_TIMER and
    # posts WM_QUIT(42) on WM_DESTROY
    wndproc = bytes.fromhex(
        "8b4424083d81000000741683f80174193d13010000742783f802744031c0c21000b801000000c210006a006a326a07ff7424"
        "10ff151018400031c0c21000ff05001c4000833d001c400003720aff742404ff151418400031c0c210006a2aff151818400031"
        "c0c21000"
    )
    return window(0x80000000, wndproc)


def window_closed_by_click() -> bytes:
    # a visible window that validates itself on WM_PAINT and posts WM_QUIT(7) when clicked
    wndproc = bytes.fromhex(
        "8b4424083d8100000074113d01020000742383f80f740d31c0c21000b801000000c210006a00ff742408ff151c18400031c0"
        "c210006a07ff151818400031c0c21000"
    )
    return window(0x90000000, wndproc)


def window_quit_from_thread() -> bytes:
    # the procedure starts a thread on WM_CREATE; the thread sends WM_USER to the window, and the procedure
    # answers it with PostQuitMessage(5)
    body = bytes.fromhex(
        "8b4424083d81000000741183f80174143d00040000742931c0c21000b801000000c210006a006a00ff74240c68801240006a00"
        "6a00ff152018400031c0c210006a05ff151818400031c0c21000"
    )
    # the thread: SendMessageA(hwnd, WM_USER, 0, 0); return 0
    worker = bytes.fromhex("6a006a006800040000ff742410ff152418400031c0c20400")
    return window(0x80000000, at(0x100, (0, body), (0x80, worker)))


def window_splash_message_loop() -> bytes:
    # a splash window at a position the system picks, waiting for a click: WM_CREATE on creation, then
    # GetMessage/DispatchMessage until the window procedure posts WM_QUIT on WM_LBUTTONDOWN
    slots, cell, wc, msg = BASE + 0x300, BASE + 0x320, BASE + 0x340, BASE + 0x380
    call = lambda idx: "ff15" + p32(slots + 4 * idx).hex()  # call [slot]
    imm = lambda value: p32(value).hex()
    default = "68" + imm(0x80000000)  # push CW_USEDEFAULT
    # RegisterClassExA(&wc); CreateWindowExA(0, "S", 0, WS_VISIBLE, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 0, 0, 0, 0)
    main = "68" + imm(wc) + call(0) + "6a00" * 4 + "6a00" + default + "6a00" + default
    main += "68" + imm(0x10000000) + "6a00" + "68" + imm(wc + 0x30) + "6a00" + call(1)
    # loop: GetMessageA(&msg, 0, 0, 0); test eax, eax; jz done; DispatchMessageA(&msg); jmp loop; done: ret
    main += "6a006a006a00" + "68" + imm(msg) + call(2) + "85c0740d" + "68" + imm(msg) + call(3) + "ebde" + "c3"
    # wndproc: count WM_CREATE; on WM_LBUTTONDOWN set the cell and PostQuitMessage(0); the rest: DefWindowProcA
    wndproc = "8b442408" + "83f801750b" + "ff05" + imm(cell + 4) + "31c0c21000"
    wndproc += "3d010200007517" + "c705" + imm(cell) + imm(1) + "6a00" + call(4) + "31c0c21000"
    wndproc += "ff25" + imm(slots + 4 * 5)
    return host(at(0x400, (0, bytes.fromhex(main)), (0x100, bytes.fromhex(wndproc))))


# Import recovery and image layout


def import_recovery_wrapped() -> bytes:
    sections = [
        (b".text", CODE_RVA, 0x1000, bytes(0x1000), 0xE0000020),
        (b".stub", 0x3000, 0x10, b"\xc3", 0x60000020),
    ]
    return build_pe(sections, entry_rva=0x3000)


def import_recovery_constants() -> bytes:
    sections = [
        (b".text", CODE_RVA, 0x1000, bytes(0x1000), 0xE0000020),
        (b".data", 0x2000, 0x8000, bytes(0x8000), 0xC0000040),
        (b".stub", 0xA000, 0x10, b"\xc3", 0x60000020),
    ]
    return build_pe(sections, entry_rva=0xA000)


def raw_data_past_virtual_size() -> bytes:
    # VirtualSize 0x45, SizeOfRawData 0x200
    raw = bytes((i * 7 + 1) & 0xFF or 1 for i in range(0x48))
    sections = [(b".text", CODE_RVA, 0x10, b"\xc3", 0xE0000020), (b".data", 0x2000, 0x45, raw, 0xC0000040)]
    return build_pe(sections, entry_rva=CODE_RVA)


def scan_for_dos_stub() -> bytes:
    # and ebx, ~0xfff; again: cmp dword [ebx + 0x4e], "This"; je done; sub ebx, 0x100; jmp again; done: ret
    return host(bytes.fromhex("81e300f0ffff 817b4e54686973 7408 81eb00010000 ebef c3"))


def importing(dll: str) -> bytes:
    """
    An image that imports ThirdParty@@ and ordinal 7 from a DLL.
    """
    idata = bytearray(0x200)
    struct.pack_into("<5I", idata, 0, 0x2080, 0, 0, 0x2100, 0x2080)  # descriptor: ILT = IAT, DLL name
    struct.pack_into("<II", idata, 0x80, 0x20C0, 0x80000007)  # ILT: hint/name, ordinal 7
    idata[0xC2:0xCF] = b"ThirdParty@@\0"
    idata[0x100 : 0x101 + len(dll)] = dll.encode() + b"\0"
    sections = [
        (b".text", CODE_RVA, 0x1000, b"\xc3", 0x60000020),
        (b".idata", 0x2000, 0x200, bytes(idata), 0xC0000040),
    ]
    return build_pe(sections, entry_rva=CODE_RVA, directories={1: (0x2000, 0x28)})


def tls(fields: tuple[int, int, int, int]) -> bytes:
    # IMAGE_TLS_DIRECTORY32 at +0x100: raw data start and end, index, callbacks, zero fill, characteristics
    code = at(0x400, (0, b"\xc3"), (0x100, struct.pack("<6I", *fields, 0, 0)))
    return host(code, directories={9: (CODE_RVA + 0x100, 0x18)})


# Exceptions


def trap_flag(stepped: str) -> bytes:
    main = bytes.fromhex(
        "b9 e8 03 00 00 49 75 fd"  # spin, so the rest runs natively
        "9c 81 0c 24 00 01 00 00 9d" + stepped + "90 c3"  # pushfd; or dword [esp], 0x100; popfd
    )
    cell = BASE + 0x100
    # vectored handler: store the exception code and address, continue execution
    handler = (
        bytes.fromhex("8b 44 24 04 8b 08 8b 11 89 15")
        + p32(cell)
        + bytes.fromhex("8b 51 0c 89 15")
        + p32(cell + 4)
        + bytes.fromhex("83 c8 ff c2 04 00")
    ).ljust(0x1E, b"\x90")
    return host(at(0x200, (0, main), (0x80, handler)))


def unhandled_exception_filter() -> bytes:
    # entry: push 0x1234; call [UnhandledExceptionFilter]; ret
    slot = BASE + 0x100
    main = b"\x68" + p32(0x1234) + b"\xff\x15" + p32(slot) + b"\xc3"
    # top-level filter: mov [cell], arg; mov eax, -1 (EXCEPTION_CONTINUE_EXECUTION); ret 4
    cell = slot + 4
    top_level = bytes.fromhex("8b442404 a3") + p32(cell) + bytes.fromhex("b8ffffffff c20400")
    return host(at(0x200, (0, main), (0x80, top_level)))


def seh_nested_frame() -> bytes:
    main = bytes.fromhex(
        "68"
        + p32(BASE + 0x40).hex()  # push handler
        + "64ff3500000000 64892500000000"  # push fs:[0]; mov fs:[0], esp
        + "31c0 8900 cc"  # xor eax, eax; mov [eax], eax; int3
    )
    resume = bytes.fromhex("648f0500000000 83c404 b807000000 c3")  # pop fs:[0]; add esp, 4; mov eax, 7; ret
    # handler: mov eax, fs:[0]; mov esp, [eax]; jmp resume
    handler = bytes.fromhex("64a100000000 8b20 e9") + struct.pack("<i", 0x20 - 0x4D)
    return host(at(0x100, (0, main), (0x20, resume), (0x40, handler)))


def rtl_unwind() -> bytes:
    cell, slot, handler_a, handler_b = BASE + 0x300, BASE + 0x2F0, BASE + 0x100, BASE + 0x120
    push_frame = lambda handler: b"\x68" + p32(handler) + bytes.fromhex("64ff3500000000 64892500000000")
    main = (
        push_frame(handler_a)
        + bytes.fromhex("89e3")  # mov ebx, esp: the target frame
        + push_frame(handler_b)
        + bytes.fromhex("6a55 6a00")  # ReturnValue, ExceptionRecord
    )
    after = BASE + len(main) + 1 + 4 + 1 + 6
    main += b"\x68" + p32(after) + b"\x53" + b"\xff\x15" + p32(slot)  # TargetIp, TargetFrame; call [RtlUnwind]
    main += (
        b"\xa3" + p32(cell + 8)  # mov [cell+8], eax
        + bytes.fromhex("64a100000000")  # mov eax, fs:[0]
        + b"\xa3" + p32(cell + 4)
        + b"\x89\x1d" + p32(cell + 16)  # mov [cell+16], ebx
        + bytes.fromhex("8b03 64a300000000 8d6308 c3")  # pop frame A, restore esp, ret
    )  # fmt: skip
    # handler B: record the ExceptionFlags and the call count, return ExceptionContinueSearch
    b = (
        bytes.fromhex("8b442404 8b4004") + b"\xa3" + p32(cell) + b"\xff\x05" + p32(cell + 12)
        + bytes.fromhex("b801000000 c3")
    )  # fmt: skip
    return host(at(0x400, (0, main), (0x100, bytes.fromhex("b801000000c3")), (0x120, b)))


def fault_on_small_stack() -> bytes:
    cell = BASE + 0x100
    # mov ebp, esp; mov esp, top; xor eax, eax; mov [eax], eax; mov esp, ebp; ret     (the test sets top)
    main = bytes.fromhex("89 e5 bc 00 00 00 00 31 c0 89 00 89 ec c3")
    # vectored handler: save CONTEXT.EFlags, skip the faulting mov, continue execution
    handler = bytes.fromhex(
        "8b 44 24 04 8b 40 04 8b 88 c0 00 00 00 89 0d" + p32(cell).hex()
        + "83 80 b8 00 00 00 02 83 c8 ff c2 04 00"
    )  # fmt: skip
    return host(at(0x204, (0, main), (0x80, handler)))


def x64_try_except() -> bytes:
    rva = CODE_RVA
    code = at(
        0x400,
        # main: sub rsp, 0x28; mov rbx, 0x7777; call faulting; nop; xor eax, eax; add rsp, 0x28; ret
        (0x00, bytes.fromhex("48 83 ec 28 48 c7 c3 77 77 00 00 e8 20 00 00 00 90 31 c0 48 83 c4 28 c3")),
        # __except block: mov [code], eax; mov [saved_rbx], rbx; add rsp, 0x28; ret
        (0x18, bytes.fromhex("89 05 ea 00 00 00 48 89 1d eb 00 00 00 48 83 c4 28 c3")),
        # faulting: push rbx; sub rsp, 0x20; mov rbx, 0x1111; xor eax, eax; mov [rax], eax; add rsp, 0x20; pop rbx; ret
        (0x30, bytes.fromhex("53 48 83 ec 20 48 c7 c3 11 11 00 00 31 c0 89 00 48 83 c4 20 5b c3")),
        (0x50, bytes.fromhex("b8 01 00 00 00 c3")),  # filter: EXCEPTION_EXECUTE_HANDLER
        (0x60, bytes.fromhex("ff 25 9a 00 00 00")),  # jmp [__imp___C_specific_handler]
        (0x200, struct.pack("<6I", rva, rva + 0x2A, rva + 0x300, rva + 0x30, rva + 0x46, rva + 0x340)),
        # main: handler flag, sub rsp 0x28; one scope around the call, with a filter
        (0x300, bytes.fromhex("09 04 01 00 04 42 00 00")),
        (0x308, struct.pack("<6I", rva + 0x60, 1, rva + 0x0B, rva + 0x11, rva + 0x50, rva + 0x18)),
        # faulting: sub rsp 0x20, push rbx
        (0x340, bytes.fromhex("01 05 02 00 05 32 01 30")),
    )
    return host(code, is_64bit=True, directories={3: (rva + 0x200, 24)})


def syscall_stub_tail() -> bytes:
    # entry: mov eax, [ZwClose + 1]; push 0x1234; call rest; ret          (the test sets the address)
    # rest:  mov edx, 0x7ffe0300; call [edx]; ret 4          (a copy of the stub minus its first instruction)
    code = b"\xa1\0\0\0\0\x68" + p32(0x1234) + b"\xe8\x01\0\0\0\xc3"
    return host(code + bytes.fromhex("ba0003fe7f ff12 c20400"))


# Packed files

TEXT_RVA, STUB_RVA = 0x1000, 0x3000
OEP_RVA = TEXT_RVA + 0x800
XOR_KEY = 0x5A


def xor_packed(use_seh: bool = False, transfer: bool = True) -> bytes:
    """
    A PE whose .text is XOR-encrypted on disk; the entry stub decrypts it in place and jumps to the OEP (or
    returns, without transfer).

    With use_seh, the stub first faults on purpose and relies on its SEH handler to skip the faulting instruction.
    """
    code = bytearray(fake_code(0x1000, IMAGE_BASE + TEXT_RVA))
    code[OEP_RVA - TEXT_RVA : OEP_RVA - TEXT_RVA + 3] = b"\x31\xc0\xc3"  # xor eax, eax; ret
    encrypted = bytes(b ^ XOR_KEY for b in code)
    stub = b""
    if use_seh:
        handler_rva = STUB_RVA + 0x80
        stub += b"\x68" + p32(IMAGE_BASE + handler_rva)  # push handler
        stub += bytes.fromhex("64ff3500000000 64892500000000")  # push fs:[0]; mov fs:[0], esp
        stub += bytes.fromhex("31c0 8900")  # xor eax, eax; mov [eax], eax
        stub += bytes.fromhex("648f0500000000 83c404")  # pop fs:[0]; add esp, 4
    stub += b"\xbe" + p32(IMAGE_BASE + TEXT_RVA)  # mov esi, .text
    stub += b"\xb9" + p32(len(code))  # mov ecx, size
    stub += bytes([0x80, 0x36, XOR_KEY, 0x46, 0xE2, 0xFA])  # xor byte [esi], key; inc esi; loop
    stub += b"\xe9" + struct.pack("<i", OEP_RVA - (STUB_RVA + len(stub) + 5))  # jmp OEP
    if use_seh:
        # handler: ctx = [esp+0xc]; ctx->Eip += 2; return ExceptionContinueExecution
        stub = stub.ljust(0x80, b"\xcc") + bytes.fromhex("8b44240c 8380b800000002 31c0 c3")
    packed = build_pe(
        [
            (b".text", TEXT_RVA, len(code), encrypted, 0xE0000020),
            (b".stub", STUB_RVA, len(stub), stub, 0x60000020),
        ],
        entry_rva=STUB_RVA,
    )
    if not transfer:
        assert packed.count(b"\xe2\xfa\xe9") == 1
        packed = packed.replace(b"\xe2\xfa\xe9", b"\xe2\xfa\xc3")
    return packed


def xor_packed_entry_in_text() -> bytes:
    # the entry stub sits inside the packed .text; the loader decrypts .text, restores the original entry code,
    # calls an API (the test fills the cell at .stub + 0x100) and jumps back to the entry point
    code = bytearray(fake_code(0x1000, IMAGE_BASE + TEXT_RVA))
    code[OEP_RVA - TEXT_RVA : OEP_RVA - TEXT_RVA + 3] = b"\x31\xc0\xc3"  # xor eax, eax; ret
    encrypted = bytearray(b ^ XOR_KEY for b in code)
    encrypted[OEP_RVA - TEXT_RVA : OEP_RVA - TEXT_RVA + 5] = b"\xe9" + struct.pack("<i", STUB_RVA - OEP_RVA - 5)
    cell = IMAGE_BASE + STUB_RVA + 0x100
    stub = b"\xbe" + p32(IMAGE_BASE + TEXT_RVA)  # mov esi, .text
    stub += b"\xb9" + p32(len(code))  # mov ecx, size
    stub += bytes([0x80, 0x36, XOR_KEY, 0x46, 0xE2, 0xFA])  # xor byte [esi], key; inc esi; loop
    oep = IMAGE_BASE + OEP_RVA
    stub += b"\xc7\x05" + struct.pack("<II", oep, 0x90C3C031)  # mov dword [oep], xor eax, eax; ret
    stub += b"\xff\x15" + p32(cell)  # call [GetTickCount]
    stub += b"\xe9" + struct.pack("<i", OEP_RVA - (STUB_RVA + len(stub) + 5))  # jmp oep
    return build_pe(
        [
            (b".text", TEXT_RVA, len(code), bytes(encrypted), 0xE0000020),
            (b" 0", STUB_RVA, 0x200, stub, 0xE0000020),
        ],
        entry_rva=OEP_RVA,
    )


LZMA_PROPS = bytes([0x5D, 0x00, 0x00, 0x00, 0x04])
RDATA_RVA, VMP0_RVA, VMP1_RVA = 0x3000, 0x4000, 0x5000
CODE = fake_code(0x1800, IMAGE_BASE + TEXT_RVA)
RDATA = b"imported strings and constants " * 64
VMP0 = bytes(range(256)) * 4


def _compress(data: bytes) -> bytes:
    filters = [{"id": lzma.FILTER_LZMA1, "lc": 3, "lp": 0, "pb": 2, "dict_size": 1 << 26}]
    compressor = lzma.LZMACompressor(lzma.FORMAT_RAW, filters=filters)
    return compressor.compress(data) + compressor.flush()


def _rol32(value: int, count: int) -> int:
    count &= 31
    return ((value << count) | (value >> (32 - count))) & 0xFFFFFFFF


def vmprotect_like(encrypted_table: bool) -> bytes:
    """
    A PE laid out like a VMProtect-packed file: empty original sections, compressed blocks and PACKER_INFO in .vmp1.
    """
    blocks = [(TEXT_RVA, _compress(CODE)), (RDATA_RVA, _compress(RDATA)), (VMP0_RVA, _compress(VMP0))]
    table_size = 8 * (len(blocks) + 1)
    loader = b"\x68\x78\x56\x34\x12\xe8\x00\x00\x00\x00".ljust(0x20, b"\xcc")
    payload_off = len(loader) + len(LZMA_PROPS) + table_size
    table = struct.pack("<II", VMP1_RVA + len(loader), 0)
    stream = bytearray()
    key = 0x5A17C3D9
    for idx, (dst, block) in enumerate(blocks):
        stored = dst ^ _rol32(key, 7 * idx) if encrypted_table else dst
        table += struct.pack("<II", VMP1_RVA + payload_off + len(stream), stored)
        stream += block
    vmp1 = loader + LZMA_PROPS + table + bytes(stream)
    return build_pe(
        [
            (b".text", TEXT_RVA, 0x1800, b"", 0x60000020),
            (b".rdata", RDATA_RVA, len(RDATA), b"", 0x40000040),
            (b".vmp0", VMP0_RVA, len(VMP0), b"", 0x60000060),
            (b".vmp1", VMP1_RVA, len(vmp1), vmp1, 0xE0000060),
        ],
        entry_rva=VMP1_RVA,
    )


I386 = {
    "host.exe": lambda: host(b"\xc3"),
    "blank_0x400.exe": lambda: host(bytes(0x400)),
    "blank_0x40.exe": lambda: host(bytes(0x40)),
    "sleep_poll_long_sleeper.exe": sleep_poll_long_sleeper,
    "sleep_poll_loop.exe": sleep_poll_loop,
    "shared_data_time.exe": shared_data_time,
    "com_shell_link.exe": com_shell_link,
    "com_allocator_stream_wmi.exe": com_allocator_stream_wmi,
    "window_timer_and_destroy.exe": window_timer_and_destroy,
    "window_closed_by_click.exe": window_closed_by_click,
    "window_quit_from_thread.exe": window_quit_from_thread,
    "window_splash_message_loop.exe": window_splash_message_loop,
    "import_recovery_wrapped.exe": import_recovery_wrapped,
    "import_recovery_constants.exe": import_recovery_constants,
    "raw_data_past_virtual_size.exe": raw_data_past_virtual_size,
    "scan_for_dos_stub.exe": scan_for_dos_stub,
    "imports_thirdparty.exe": lambda: importing("thirdparty.dll"),
    "imports_api_set.exe": lambda: importing("api-ms-win-core-synch-l1-2-0.dll"),
    "tls_bad_raw_data.exe": lambda: tls((0x59190000, 0x59190100, BASE + 0x200, 0)),
    "tls_bad_index.exe": lambda: tls((BASE + 0x300, BASE + 0x310, 0x30D26308, 0)),
    "tls_bad_callback_list.exe": lambda: tls((0, 0, BASE + 0x200, 0x7FFE0000)),
    "tls_inside_image.exe": lambda: tls((BASE + 0x300, BASE + 0x310, BASE + 0x200, 0)),
    "trap_flag_nop.exe": lambda: trap_flag("90"),
    "trap_flag_cpuid.exe": lambda: trap_flag("0f a2"),
    "hlt.exe": lambda: host(b"\x90\xf4\xc3"),  # nop; hlt; ret
    "cli.exe": lambda: host(b"\x90\xfa\xc3"),
    "sti.exe": lambda: host(b"\x90\xfb\xc3"),
    "in.exe": lambda: host(b"\x90\xec\xc3"),  # in al, dx
    "out.exe": lambda: host(b"\x90\xee\xc3"),  # out dx, al
    "mov_eax_7.exe": lambda: host(b"\xb8\x07\x00\x00\x00\xc3"),
    "unhandled_exception_filter.exe": unhandled_exception_filter,
    "seh_nested_frame.exe": seh_nested_frame,
    "rtl_unwind.exe": rtl_unwind,
    "fault_on_small_stack.exe": fault_on_small_stack,
    "syscall_stub_tail.exe": syscall_stub_tail,
    "xor_packed.exe": xor_packed,
    "xor_packed_seh.exe": lambda: xor_packed(use_seh=True),
    "xor_packed_no_transfer.exe": lambda: xor_packed(transfer=False),
    "xor_packed_entry_in_text.exe": xor_packed_entry_in_text,
    "vmprotect_like_plain_table.exe": lambda: vmprotect_like(False),
    "vmprotect_like_encrypted_table.exe": lambda: vmprotect_like(True),
    "not_packed.exe": lambda: build_pe([(b".text", TEXT_RVA, len(CODE), CODE, 0x60000020)], entry_rva=TEXT_RVA),
    "not_packed_0x2000.exe": lambda: build_pe([(b".text", TEXT_RVA, 0x2000, CODE, 0x60000020)], entry_rva=TEXT_RVA),
}

X86_64 = {
    "host.exe": lambda: host(b"\xc3", is_64bit=True),
    "try_except.exe": x64_try_except,
}


def main() -> None:
    root = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), "..", "..", "tests")
    for arch, builders in (("i386", I386), ("x86_64", X86_64)):
        directory = os.path.join(root, arch, "windows", "unpacker")
        os.makedirs(directory, exist_ok=True)
        for name, build in builders.items():
            with open(os.path.join(directory, name), "wb") as f:
                f.write(build())


if __name__ == "__main__":
    main()
