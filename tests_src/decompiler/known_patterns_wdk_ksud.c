/*
 * Test binary for angr's KnownPatternFinder: reads of KUSER_SHARED_DATA, the
 * shared user data page Windows maps read-only at the fixed address
 * 0x7FFE0000 in every user-mode process (x86 and x64 alike). ntdll, kernel32
 * and countless drivers/loaders read its fields through the ntddk.h
 * SharedUserData macro, which compiles to a bare load from an absolute
 * constant address -- the address itself identifies the field.
 *
 * The struct layout below reproduces the offsets of the WDK ddk/ntddk.h
 * KUSER_SHARED_DATA (mingw-w64 ships the same header; the offsets were
 * verified with offsetof on both i686 and x86_64). Each field read is
 * exercised through a noinline wrapper so the load survives as its own
 * function, and every read is volatile so the compiler can neither fold two
 * reads of the same field together nor hoist them out.
 *
 * Build: make -f Makefile.known_patterns
 * x86_64-w64-mingw32-gcc -O2 -o known_patterns_wdk_ksud.exe  known_patterns_wdk_ksud.c
 * i686-w64-mingw32-gcc   -O2 -o known_patterns_wdk_ksud32.exe known_patterns_wdk_ksud.c
 */
#include <windows.h>

/* winnt.h/ntddk.h: the user-mode mapping of KUSER_SHARED_DATA. Kernel-mode
 * code uses KI_USER_SHARED_DATA (0xffdf0000 on x86, 0xfffff78000000000 on
 * x64) instead; user mode always sees the page here. */
#define USER_SHARED_DATA 0x7FFE0000

#define KSUD_U8(off)  (*(volatile UCHAR *)(ULONG_PTR)(USER_SHARED_DATA + (off)))
#define KSUD_U16(off) (*(volatile USHORT *)(ULONG_PTR)(USER_SHARED_DATA + (off)))
#define KSUD_U32(off) (*(volatile ULONG *)(ULONG_PTR)(USER_SHARED_DATA + (off)))
#define KSUD_U64(off) (*(volatile ULONG64 *)(ULONG_PTR)(USER_SHARED_DATA + (off)))

/* --- time ------------------------------------------------------------- */

__attribute__((noinline)) ULONG ksud_tick_count_low(void)
{
    return KSUD_U32(0x000); /* TickCountLowDeprecated */
}

__attribute__((noinline)) ULONG ksud_tick_count_multiplier(void)
{
    return KSUD_U32(0x004); /* TickCountMultiplier */
}

__attribute__((noinline)) ULONG64 ksud_interrupt_time(void)
{
    return KSUD_U64(0x008); /* InterruptTime (KSYSTEM_TIME, read as a quad) */
}

__attribute__((noinline)) ULONG ksud_interrupt_time_low(void)
{
    return KSUD_U32(0x008); /* InterruptTime.LowPart */
}

__attribute__((noinline)) ULONG ksud_system_time_low(void)
{
    return KSUD_U32(0x014); /* SystemTime.LowPart */
}

__attribute__((noinline)) ULONG ksud_time_zone_bias_low(void)
{
    return KSUD_U32(0x020); /* TimeZoneBias.LowPart */
}

__attribute__((noinline)) ULONG ksud_tick_count_low_new(void)
{
    return KSUD_U32(0x320); /* TickCount.LowPart (the modern TickCountLow) */
}

__attribute__((noinline)) ULONG64 ksud_tick_count_quad(void)
{
    return KSUD_U64(0x320); /* TickCountQuad */
}

/* the inlined GetTickCount() of ntdll: (TickCountQuad * Multiplier) >> 24 */
__attribute__((noinline)) ULONG ksud_get_tick_count(void)
{
    return (ULONG)((KSUD_U64(0x320) * (ULONG64)KSUD_U32(0x004)) >> 24);
}

/* --- image / system identity ------------------------------------------ */

__attribute__((noinline)) USHORT ksud_image_number_low(void)
{
    return KSUD_U16(0x02C); /* ImageNumberLow */
}

__attribute__((noinline)) USHORT ksud_image_number_high(void)
{
    return KSUD_U16(0x02E); /* ImageNumberHigh */
}

/* negative control: a read from inside the same page at an offset the pattern
 * table deliberately does not cover (the NtSystemRoot WCHAR array); it must
 * stay a raw load */
__attribute__((noinline)) USHORT ksud_nt_system_root_first_wchar(void)
{
    return KSUD_U16(0x030); /* NtSystemRoot[0] */
}

__attribute__((noinline)) ULONG ksud_time_zone_id(void)
{
    return KSUD_U32(0x240); /* TimeZoneId */
}

__attribute__((noinline)) ULONG ksud_large_page_minimum(void)
{
    return KSUD_U32(0x244); /* LargePageMinimum */
}

__attribute__((noinline)) ULONG ksud_nt_product_type(void)
{
    return KSUD_U32(0x264); /* NtProductType */
}

__attribute__((noinline)) ULONG ksud_nt_major_version(void)
{
    return KSUD_U32(0x26C); /* NtMajorVersion */
}

__attribute__((noinline)) ULONG ksud_nt_minor_version(void)
{
    return KSUD_U32(0x270); /* NtMinorVersion */
}

/* the classic "is this at least Windows <maj>.<min>" version test: two field
 * reads that the compiler folds into a single expression */
__attribute__((noinline)) ULONG ksud_version_combined(void)
{
    return KSUD_U32(0x26C) * 100 + KSUD_U32(0x270);
}

/* two field reads that stay in separate basic blocks */
__attribute__((noinline)) ULONG ksud_version_branch(void)
{
    if (KSUD_U32(0x26C) > 6)
        return 1;
    if (KSUD_U32(0x270) > 1)
        return 2;
    return 0;
}

__attribute__((noinline)) ULONG ksud_suite_mask(void)
{
    return KSUD_U32(0x2D0); /* SuiteMask */
}

__attribute__((noinline)) UCHAR ksud_kd_debugger_enabled(void)
{
    return KSUD_U8(0x2D4); /* KdDebuggerEnabled */
}

__attribute__((noinline)) ULONG ksud_active_console_id(void)
{
    return KSUD_U32(0x2D8); /* ActiveConsoleId */
}

__attribute__((noinline)) ULONG ksud_number_of_physical_pages(void)
{
    return KSUD_U32(0x2E8); /* NumberOfPhysicalPages */
}

__attribute__((noinline)) UCHAR ksud_safe_boot_mode(void)
{
    return KSUD_U8(0x2EC); /* SafeBootMode */
}

__attribute__((noinline)) ULONG ksud_shared_data_flags(void)
{
    return KSUD_U32(0x2F0); /* SharedDataFlags */
}

__attribute__((noinline)) ULONG ksud_cookie(void)
{
    return KSUD_U32(0x330); /* Cookie */
}

/* --- the kernel-mode mapping of the same page ------------------------- */

/* wdm.h: kernel-mode code reaches KUSER_SHARED_DATA through
 * KI_USER_SHARED_DATA rather than 0x7FFE0000 */
#ifdef _WIN64
#define KI_USER_SHARED_DATA 0xFFFFF78000000000ULL
#else
#define KI_USER_SHARED_DATA 0xFFDF0000UL
#endif
#define KM_KSUD_U32(off) (*(volatile ULONG *)(ULONG_PTR)(KI_USER_SHARED_DATA + (off)))

__attribute__((noinline)) ULONG ksud_km_tick_count_low(void)
{
    return KM_KSUD_U32(0x000); /* SharedUserData->TickCountLowDeprecated, kernel mode */
}

__attribute__((noinline)) ULONG ksud_km_nt_major_version(void)
{
    return KM_KSUD_U32(0x26C); /* SharedUserData->NtMajorVersion, kernel mode */
}

/* --- NTSTATUS predicates (ntdef.h) ------------------------------------ */

__attribute__((noinline)) int ntstatus_success(LONG status)
{
    return status >= 0; /* NT_SUCCESS */
}

__attribute__((noinline)) int ntstatus_information(LONG status)
{
    return (((ULONG)status) >> 30) == 1; /* NT_INFORMATION */
}

__attribute__((noinline)) int ntstatus_warning(LONG status)
{
    return (((ULONG)status) >> 30) == 2; /* NT_WARNING */
}

__attribute__((noinline)) int ntstatus_error(LONG status)
{
    return (((ULONG)status) >> 30) == 3; /* NT_ERROR */
}

/* the same predicates in their usual branch context; the branches call an
 * opaque sink so the compiler cannot flatten them back into an expression */
void sink(int x);

__attribute__((noinline)) int ntstatus_error_if(LONG status)
{
    if ((((ULONG)status) >> 30) == 3)
    {
        sink(1);
        return 1;
    }
    sink(2);
    return 0;
}

__attribute__((noinline)) int ntstatus_warning_if(LONG status)
{
    if ((((ULONG)status) >> 30) == 2)
    {
        sink(3);
        return 1;
    }
    sink(4);
    return 0;
}

__attribute__((noinline)) int ntstatus_information_if(LONG status)
{
    if ((((ULONG)status) >> 30) == 1)
    {
        sink(5);
        return 1;
    }
    sink(6);
    return 0;
}

__attribute__((noinline)) int ntstatus_success_if(LONG status)
{
    if (status >= 0)
    {
        sink(7);
        return 1;
    }
    sink(8);
    return 0;
}

/* --- current thread / process accessors (winnt.h intrinsics) ---------- */

__attribute__((noinline)) void *nt_current_teb(void)
{
#ifdef _WIN64
    return (void *)__readgsqword(0x30);
#else
    return (void *)(ULONG_PTR)__readfsdword(0x18);
#endif
}

__attribute__((noinline)) void *nt_current_peb(void)
{
#ifdef _WIN64
    return (void *)__readgsqword(0x60);
#else
    return (void *)(ULONG_PTR)__readfsdword(0x30);
#endif
}

#ifdef _WIN64
__attribute__((noinline)) void *ke_get_current_thread(void)
{
    return (void *)__readgsqword(0x188);
}
#endif

volatile int g_sink;

__attribute__((noinline)) void sink(int x)
{
    g_sink = x;
}

int main(void)
{
    ULONG acc = 0;
    acc += ksud_tick_count_low();
    acc += ksud_tick_count_multiplier();
    acc += (ULONG)ksud_interrupt_time();
    acc += ksud_interrupt_time_low();
    acc += ksud_system_time_low();
    acc += ksud_time_zone_bias_low();
    acc += ksud_tick_count_low_new();
    acc += (ULONG)ksud_tick_count_quad();
    acc += ksud_get_tick_count();
    acc += ksud_image_number_low();
    acc += ksud_image_number_high();
    acc += ksud_nt_system_root_first_wchar();
    acc += ksud_time_zone_id();
    acc += ksud_large_page_minimum();
    acc += ksud_nt_product_type();
    acc += ksud_nt_major_version();
    acc += ksud_nt_minor_version();
    acc += ksud_version_combined();
    acc += ksud_version_branch();
    acc += ksud_km_tick_count_low();
    acc += ksud_km_nt_major_version();
    acc += ksud_suite_mask();
    acc += ksud_kd_debugger_enabled();
    acc += ksud_active_console_id();
    acc += ksud_number_of_physical_pages();
    acc += ksud_safe_boot_mode();
    acc += ksud_shared_data_flags();
    acc += ksud_cookie();
    acc += (ULONG)ntstatus_success((LONG)acc);
    acc += (ULONG)ntstatus_information((LONG)acc);
    acc += (ULONG)ntstatus_warning((LONG)acc);
    acc += (ULONG)ntstatus_error((LONG)acc);
    acc += (ULONG)ntstatus_error_if((LONG)acc);
    acc += (ULONG)ntstatus_warning_if((LONG)acc);
    acc += (ULONG)ntstatus_information_if((LONG)acc);
    acc += (ULONG)ntstatus_success_if((LONG)acc);
    acc += (ULONG)(ULONG_PTR)nt_current_teb();
    acc += (ULONG)(ULONG_PTR)nt_current_peb();
#ifdef _WIN64
    acc += (ULONG)(ULONG_PTR)ke_get_current_thread();
#endif
    return (int)acc;
}
