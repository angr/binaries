# Rebuild the PE fixture whose real-mode DOS stub places e_lfanew beyond 4 KiB.
#
# Open Watcom is pinned to the same nixpkgs revision as tests_src/dos. Run this
# Makefile in the workspace development environment for its pinned clang and
# lld-link. The stub and PE entry point link no runtime or SDK libraries.

NIXPKGS := github:NixOS/nixpkgs/42f17a57f4f6e33b3de3dca0a2a5ea5233169d02
OUTPUT := $(abspath ../../../tests/x86_64/windows/pe_large_dos_stub.exe)

.PHONY: all
all:
	set -eu; \
	scratch=$$(mktemp -d); \
	trap 'rm -rf "$$scratch"' EXIT; \
	export scratch; \
	NIXPKGS_ALLOW_UNFREE=1 nix shell --impure "$(NIXPKGS)#open-watcom-bin" --command sh -c ' \
		wasm -zq -0 -zld -fo="$$scratch/dos_stub.obj" dos_stub.asm; \
		wlink system dos option stack=512 name "$$scratch/dos_stub.exe" file "$$scratch/dos_stub.obj" \
	'; \
	clang --target=x86_64-unknown-windows-msvc -c -O1 -ffreestanding \
		-fno-stack-protector -o "$$scratch/main.obj" main.c; \
	lld-link /entry:mainCRTStartup /subsystem:console /machine:x64 /nodefaultlib \
		/timestamp:0 /Brepro "/stub:$$scratch/dos_stub.exe" "/out:$(OUTPUT)" "$$scratch/main.obj"; \
	sha256sum "$(OUTPUT)"
