/*
 * Simple test application for Windows Thread Local Storage (TLS). It
 * demonstrates both the use of TLS callbacks and TLS variables.
 *
 * Program flow:
 *  1. When the main thread is created tls_callback is called and incrememnts
 *     the TLS variable. In the main thread tls_i == 0xab
 *  2. The main thread then creates myThread. Windows will then initialize a
 *     new TLS data area for this thread. tls_i == 0xaa
 *  3. tls_callback is called in myThread and increments tls_i. In myThread
 *     tls_i == 0xab
 *  4. myThread's entry point, thread_func, is then called and decrements
 *     tls_i. tls_i == 0xaa. Note that in the main thread, tls_i == 0xab
 *  5. thread_func returns and myThread is terminated. Its tls_i is
 *     essentially deallocated
 */

#include <Windows.h>

__declspec(thread) int tls_i = 0xaa;

void NTAPI tls_callback(PVOID h, DWORD reason, PVOID reserved) {
	tls_i++;
}

#pragma data_seg(".CRT$XLB")
PIMAGE_TLS_CALLBACK p_thread_callback = tls_callback;
#pragma data_seg()

DWORD WINAPI thread_func(LPVOID lpParameter) {
	tls_i--;

	return 0;
}

int main(int argc, char *argv[]) {
	HANDLE myThread;

	myThread = CreateThread(NULL, 0, thread_func, NULL, 0, NULL);
	WaitForSingleObject(myThread, INFINITE);

	// tls_i == 0xab in the main thread at this point in time

	return 0;
}