#!/usr/bin/env bash
TARGET_DIR="../../../../binaries/tests/riscv64"
MC="llvm-mc"
LLD="ld.lld"
TRIPLE="riscv64-unknown-elf"
MC_FLAGS="-filetype=obj -triple=$TRIPLE -mattr=-relax"

clean_up() {
    echo "Cleaning up old build files..."
    rm -f "${TARGET_DIR:?Target directory not set} "/*.{o,so}
}

build_all() {
    echo "Starting compilation of RISC-V 64 relocation test cases..."

    $MC $MC_FLAGS riscv-call.s -o riscv-call.rv64.o
    $MC $MC_FLAGS riscv-hi20-lo12.s -o riscv-hi20-lo12.rv64.o
    $MC $MC_FLAGS riscv-jal.s -o riscv-jal.rv64.o
    $MC $MC_FLAGS riscv-branch.s -o riscv-branch.rv64.o
    $MC $MC_FLAGS riscv-pcrel-hilo.s -o riscv-pcrel-hilo.rv64.o
    $MC $MC_FLAGS riscv-plt.s -o riscv-plt.64.o
    
    $MC -filetype=obj -triple=riscv64 -position-independent riscv-reloc-got.s -o riscv-reloc-got.o
    $MC -filetype=obj -triple=riscv64 riscv64-reloc-64-pic.s -o riscv64-reloc-64-pic.o
    $LLD -shared riscv64-reloc-64-pic.o -o riscv64-reloc-64-pic.so

    $MC -filetype=obj -triple=riscv64 riscv-reloc-copy.s -o riscv-reloc-copy.o
}

move_to_binaries() {
    if [ ! -d "$TARGET_DIR" ]; then
        echo "Creating target directory: $TARGET_DIR"
        mkdir -p "$TARGET_DIR"
    fi
    
    echo "Moving files to $TARGET_DIR ..."
    # Move .o and .so files, suppressing errors if files are missing
    mv -v *.o *.so "$TARGET_DIR/" 2>/dev/null
}

clean_up

build_all

move_to_binaries

echo "All tasks completed successfully!"
