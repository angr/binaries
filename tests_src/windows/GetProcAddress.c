#include <windows.h>

typedef int (WINAPI *MESSAGEBOX)(HWND, LPCSTR, LPCSTR, UINT);
MESSAGEBOX pMessageBox = NULL;

int main() {
    HMODULE hModule = LoadLibrary("user32.dll");
    if (hModule) {
        pMessageBox = (MESSAGEBOX)GetProcAddress(hModule, "MessageBoxA");
        if (pMessageBox) {
            pMessageBox(NULL, "Hello, world!", "Dynamic MessageBox", MB_OK);
        }
        FreeLibrary(hModule);
    }
    return 0;
}
