CC := gcc
SRC := fp_basic.c

# i386 x87 (all FP via x87 stack)
I386_FLAGS := -m32 -mfpmath=387 -fno-pic -no-pie

# amd64 x87 (forced x87 via -mfpmath=387)
AMD64_X87_FLAGS := -mlong-double-80 -mfpmath=387 -mno-sse2 -no-pie

# amd64 default (SSE FP)
AMD64_SSE_FLAGS := -no-pie

# amd64 x87 math instructions (fsin, fprem, fyl2x, ...): -ffast-math lets gcc inline libm calls
X87_MATH_FLAGS := -O1 -mfpmath=387 -ffast-math -fno-inline -no-pie

BINS := \
	x87_env_amd64 \
	x87_env_x86.exe \
	fp_basic_i386_O0 \
	fp_basic_i386_O1 \
	fp_basic_amd64_O0 \
	fp_basic_amd64_O1 \
	fp_basic_amd64_default_O0 \
	fp_basic_amd64_default_O1

# Hand-crafted assembly (.o relocatable objects)
ASM_BINS := \
	four_doubles_i386.o \
	ftop_conflict_i386.o \
	slot_reuse_amd64.o \
	slot_reuse_i386.o \
	x87_sign_merge_amd64.o \
	float_bits_i386.o \
	sse_round_amd64.o \
	x87_call_delta_i386.o \
	x87_ret_proto_win32.exe \
	x87_int64_copy_win32.exe \
	x87_dclass_win32.exe \
	cvtsi2sd_signed_amd64.o \
	signed_compare_amd64.o \
	fp_reg_view_amd64.o \
	x87_fprem_i386.o \
	x87_fptan_i386.o \
	x87_fp_probe_i386.o \
	x87_stack_args_i386.o \
	x87_narrow_store_i386.o \
	x87_sibling_store_i386.o

all: $(BINS) $(ASM_BINS) $(OBJ_BINS) x87_math_amd64 x87_ld_local_i386_O0.o libm_names_amd64

x87_math_amd64: x87_math.c
	$(CC) $(X87_MATH_FLAGS) -o $@ $< -lm

# local functions named after libm functions with mixed int/FP parameters
libm_names_amd64: libm_names.c
	$(CC) -O1 -fno-builtin -fno-inline -no-pie -o $@ $<

# x87 long double stack local (fstpt/fldt) narrowed to float
x87_ld_local_i386_O0.o: x87_ld_local.c
	$(CC) -O0 -m32 -mfpmath=387 -fno-pic -c -o $@ $<

fp_basic_i386_O0: $(SRC)
	$(CC) -O0 $(I386_FLAGS) -o $@ $<

fp_basic_i386_O1: $(SRC)
	$(CC) -O1 $(I386_FLAGS) -o $@ $<

fp_basic_amd64_O0: $(SRC)
	$(CC) -O0 $(AMD64_X87_FLAGS) -o $@ $<

fp_basic_amd64_O1: $(SRC)
	$(CC) -O1 $(AMD64_X87_FLAGS) -o $@ $<

fp_basic_amd64_default_O0: $(SRC)
	$(CC) -O0 $(AMD64_SSE_FLAGS) -o $@ $<

fp_basic_amd64_default_O1: $(SRC)
	$(CC) -O1 $(AMD64_SSE_FLAGS) -o $@ $<

# x87/SSE control-state instructions (fnstenv/fnsave/fxsave/fninit/rdtsc) lifted to dirty helpers
x87_env_amd64: x87_env.c
	$(CC) -O1 $(AMD64_SSE_FLAGS) -o $@ $<

x87_env_x86.exe: x87_env.c
	i686-w64-mingw32-gcc -O1 -o $@ $<

four_doubles_i386.o: four_doubles_i386.s
	nasm -f elf32 -o $@ $<

ftop_conflict_i386.o: ftop_conflict_i386.s
	nasm -f elf32 -o $@ $<

x87_fp_probe_i386.o: x87_fp_probe_i386.s
	nasm -f elf32 -o $@ $<

x87_narrow_store_i386.o: x87_narrow_store_i386.s
	nasm -f elf32 -o $@ $<

x87_sibling_store_i386.o: x87_sibling_store_i386.s
	nasm -f elf32 -o $@ $<

slot_reuse_amd64.o: slot_reuse_amd64.s
	nasm -f elf64 -o $@ $<

slot_reuse_i386.o: slot_reuse_i386.s
	nasm -f elf32 -o $@ $<

x87_sign_merge_amd64.o: x87_sign_merge_amd64.s
	nasm -f elf64 -o $@ $<

float_bits_i386.o: float_bits_i386.s
	nasm -f elf32 -o $@ $<

vec_convert_amd64.o: vec_convert_amd64.s
	nasm -f elf64 -o $@ $<

# roundsd/roundss with constant and MXCSR rounding modes
sse_round_amd64.o: sse_round_amd64.s
	nasm -f elf64 -o $@ $<

# movmskpd/movmskps on genuine vectors and on scalars
sse_movmsk_amd64.o: sse_movmsk_amd64.s
	nasm -f elf64 -o $@ $<

# genuine 128-bit vectors: pcmpgtd/pshufd 64-bit compare, packs, paddq, packuswb, addps
sse_m128_amd64.o: sse_m128_amd64.s
	nasm -f elf64 -o $@ $<

fp_reg_view_amd64.o: fp_reg_view_amd64.s
	nasm -f elf64 -o $@ $<

x87_fnstsw_i386.o: x87_fnstsw_i386.s
	nasm -f elf32 -o $@ $<

# fnstsw stored to memory as a value
x87_fnstsw_store_i386.o: x87_fnstsw_store_i386.s
	nasm -f elf32 -o $@ $<

x87_fcom_i386_O1.o: x87_fcom.c
	$(CC) -O1 $(I386_FNSTSW_FLAGS) -c -o $@ $<

x87_call_delta_i386.o: x87_call_delta_i386.s
	nasm -f elf32 -o $@ $<

x87_fprem_i386.o: x87_fprem_i386.s
	nasm -f elf32 -o $@ $<

x87_fptan_i386.o: x87_fptan_i386.s
	nasm -f elf32 -o $@ $<

# arguments passed and popped on the x87 stack (MSVC _CI*/_ftol helpers), in-place st(0) returns, tail calls
x87_stack_args_i386.o: x87_stack_args_i386.s
	nasm -f elf32 -o $@ $<

# Microsoft x64 float argument copied whole into xmm6 (movaps) and consumed as 32-bit lanes
float_arg_copy_win64.exe: float_arg_copy_win64.s
	nasm -f win64 -o float_arg_copy_win64.obj $<
	x86_64-w64-mingw32-gcc -nostdlib -nostartfiles -Wl,-e,start -o $@ float_arg_copy_win64.obj
	rm -f float_arg_copy_win64.obj

# X86 Windows x87 return values (st0) next to incidental eax writes, __fastcall and __cdecl
x87_ret_proto_win32.exe: x87_ret_proto_win32.s
	nasm -f win32 -o x87_ret_proto_win32.obj $<
	i686-w64-mingw32-gcc -nostdlib -nostartfiles -Wl,-e,start -o $@ x87_ret_proto_win32.obj
	rm -f x87_ret_proto_win32.obj

x87_dclass_win32.exe: x87_dclass_win32.s
	nasm -f win32 -o x87_dclass_win32.obj $<
	i686-w64-mingw32-gcc -nostdlib -nostartfiles -Wl,-e,start -o $@ x87_dclass_win32.obj
	rm -f x87_dclass_win32.obj

# X86 Windows int64 copy through fild/fistp qword (MSVC InterlockedExchange64-style store)
x87_int64_copy_win32.exe: x87_int64_copy_win32.s
	nasm -f win32 -o x87_int64_copy_win32.obj $<
	i686-w64-mingw32-gcc -nostdlib -nostartfiles -Wl,-e,start -o $@ x87_int64_copy_win32.obj
	rm -f x87_int64_copy_win32.obj

# cvtsi2sd on signed, unsigned and pointer operands
cvtsi2sd_signed_amd64.o: cvtsi2sd_signed.c
	$(CC) -O1 -c -o $@ $<

signed_compare_amd64.o: signed_compare.c
	$(CC) -O1 -fno-inline -fcf-protection=none -fno-ipa-cp -fno-ipa-sra -c -o $@ $<

clean:
	rm -f $(BINS) $(ASM_BINS) $(OBJ_BINS) x87_math_amd64 x87_ld_local_i386_O0.o libm_names_amd64

.PHONY: all clean

# a jcc whose flags come from compares in two predecessors (jump threading); stmxcsr/fnstcw reads
ASM_BINS += flag_join_i386.o
all: flag_join_i386.o

flag_join_i386.o: flag_join_i386.s
	nasm -f elf32 -o $@ $<

# branches and flag reads on flags live on function entry
ASM_BINS += flags_livein_amd64.o flags_livein_i386.o
all: flags_livein_amd64.o flags_livein_i386.o

flags_livein_amd64.o: flags_livein_amd64.s
	nasm -f elf64 -o $@ $<

flags_livein_i386.o: flags_livein_i386.s
	nasm -f elf32 -o $@ $<

# i386 64-bit integer returns in edx:eax, next to an int function that leaves edx defined
OBJ_BINS += edx_eax_ret_i386.o
all: edx_eax_ret_i386.o

edx_eax_ret_i386.o: edx_eax_ret.c
	$(CC) -O1 -m32 -mfpmath=387 -fno-pic -fno-inline -fcf-protection=none -c -o $@ $<

# adc/sbb carry chains (carry-in from an ADC/SBB flags thunk)
ASM_BINS += adc_sbb_amd64.o adc_sbb_i386.o
all: adc_sbb_amd64.o adc_sbb_i386.o

ASM_BINS += adc_chain_amd64.o
all: adc_chain_amd64.o

adc_chain_amd64.o: adc_chain_amd64.s
	nasm -f elf64 -o $@ $<

adc_sbb_amd64.o: adc_sbb_amd64.s
	nasm -f elf64 -o $@ $<

adc_sbb_i386.o: adc_sbb_i386.s
	nasm -f elf32 -o $@ $<

ASM_BINS += fp_ret_via_rax_amd64.o
all: fp_ret_via_rax_amd64.o

fp_ret_via_rax_amd64.o: fp_ret_via_rax_amd64.s
	nasm -f elf64 -o $@ $<

ASM_BINS += sret_ptr_amd64.o
all: sret_ptr_amd64.o

sret_ptr_amd64.o: sret_ptr_amd64.s
	nasm -f elf64 -o $@ $<

# a double above FLT_MAX narrowed to float, linked at address 0 as a raw AArch64 image (load it as a blob)
OBJ_BINS += fp_narrow_overflow_aarch64.bin
all: fp_narrow_overflow_aarch64.bin

fp_narrow_overflow_aarch64.bin: fp_narrow_overflow.c
	clang --target=aarch64-none-elf -O1 -ffreestanding -fno-pic -c -o fp_narrow_overflow_aarch64.o $<
	ld.lld --image-base=0 -Ttext=0 --oformat=binary -e narrow_big -o $@ fp_narrow_overflow_aarch64.o
	rm -f fp_narrow_overflow_aarch64.o
