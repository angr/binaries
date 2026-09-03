# RISC-V UEFI images

`HighMemDxe.efi` is the `OvmfPkg/Fdt/HighMemDxe` driver of the RISC-V virt
firmware, extracted from the firmware volume in
`usr/share/qemu-efi-riscv64/RISCV_VIRT_CODE.fd` as shipped by Debian's
`qemu-efi-riscv64` 2025.11-5:

    https://snapshot.debian.org/package/edk2/2025.11-5/

The build path the image carries in its debug directory names the module and the
package it came from:

    /build/reproducible-path/edk2-2025.11/debian/build/qemu-efi-riscv64/default/
    Build/RiscVVirtQemu/RELEASE_GCC5/RISCV64/OvmfPkg/Fdt/HighMemDxe/HighMemDxe/
    DEBUG/HighMemDxe.dll

sha256 `6a62b50f6a7ba104994c7bbc0219f8391729417731426c56d4d417c4173f4e9f`,
8832 bytes, PE32+, machine `0x5064` (RISCV64), subsystem 11
(`EFI_BOOT_SERVICE_DRIVER`).

edk2 is BSD-2-Clause-Patent; redistribution is allowed.
