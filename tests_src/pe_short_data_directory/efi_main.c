/* Minimal UEFI application body: returns EFI_SUCCESS immediately. */
typedef unsigned long long efi_status_t;

const char efi_fixture_tag[] = "cle short data directory fixture";

efi_status_t efi_main(void *image_handle, void *system_table)
{
	(void)image_handle;
	(void)system_table;
	return 0;
}
