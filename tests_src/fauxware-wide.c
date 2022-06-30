#include <windows.h>
#include <string.h>

const wchar_t *sneaky = L"SOSNEAKY";
const wchar_t *msg_welcome = L"Welcome to the admin console, trusted user!\n";
const wchar_t *msg_goaway = L"Go away!\n";
const wchar_t *msg_username = L"Username: \n";
const wchar_t *msg_password = L"Password: \n";

int authenticate(wchar_t *username, wchar_t *password) {
	wchar_t stored_pw[9];
	stored_pw[8] = 0;
	HANDLE pwfile;

	// evil back d00r
	if (wcscmp(password, sneaky) == 0) return 1;

	pwfile = CreateFileW(username, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pwfile == INVALID_HANDLE_VALUE) return 0;
	ReadFile(pwfile, stored_pw, 8*sizeof(wchar_t), NULL, NULL);
	CloseHandle(pwfile);

	if (wcscmp(password, stored_pw) == 0) return 1;
	return 0;
}

void accepted() {
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), msg_welcome, wcslen(msg_welcome), NULL, NULL);
}

void rejected() {
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), msg_goaway, wcslen(msg_goaway), NULL, NULL);
	exit(1);
}

int main(int argc, char **argv)
{
	wchar_t username[9];
	wchar_t password[9];
	int authed;
	DWORD count;

	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), msg_username, wcslen(msg_username), NULL, NULL);
	ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), username, 9, &count, NULL);
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), msg_password, wcslen(msg_password), NULL, NULL);
	ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), password, 9, &count, NULL);

	username[8] = 0;
	password[8] = 0;

	authed = authenticate(username, password);
	if (authed) accepted();
	else rejected();
}
