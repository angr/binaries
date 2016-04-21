#include <Windows.h>

__declspec(thread) int tls_i = 0xaa;

void NTAPI tls_callback(PVOID h, DWORD reason, PVOID reserved) {
	tls_i++;
}

#pragma data_seg(".CRT$XLB")
PIMAGE_TLS_CALLBACK p_thread_callback = tls_callback;

// Reset the default section
#pragma data_seg()

int main(int argc, char *argv[]) {
	if (tls_i == 0xab) {
		return 0;
	}
	else {
		return 1;
	}
}