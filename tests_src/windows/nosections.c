/* A console program with no CRT, built by build_nosections.py. */

typedef unsigned long DWORD;
typedef void *HANDLE;

__declspec(dllimport) HANDLE __stdcall GetStdHandle(DWORD std_handle);
__declspec(dllimport) int __stdcall WriteFile(HANDLE file, const void *buffer, DWORD length, DWORD *written,
                                              void *overlapped);
__declspec(dllimport) DWORD __stdcall GetTickCount(void);
__declspec(dllimport) void __stdcall ExitProcess(unsigned int code);

static const char digits[] = "0123456789abcdef";

static __attribute__((noinline)) DWORD format_hex(DWORD value, char *out)
{
    for (int i = 7; i >= 0; i--) {
        out[i] = digits[value & 0xf];
        value >>= 4;
    }
    out[8] = '\n';
    return 9;
}

static __attribute__((noinline)) int write_all(HANDLE out, const char *buffer, DWORD length)
{
    DWORD written = 0;
    while (length != 0) {
        if (!WriteFile(out, buffer, length, &written, 0) || written == 0)
            return 0;
        buffer += written;
        length -= written;
    }
    return 1;
}

void __stdcall start(void)
{
    char line[9];
    HANDLE out = GetStdHandle((DWORD)-11);
    DWORD length = format_hex(GetTickCount(), line);
    ExitProcess(write_all(out, line, length) ? 0 : 1);
}
