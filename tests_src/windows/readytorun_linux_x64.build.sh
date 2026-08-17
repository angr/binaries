#!/bin/sh
# Build tests/x86_64/readytorun_linux_x64.dll, a .NET ReadyToRun assembly published for
# linux-x64.
#
# ReadyToRun compiles the managed code ahead of time and stores the native code in a PE
# alongside the IL. When the target is not Windows, the compiler exclusive-ors the COFF machine
# type with a constant naming the operating system, so this image reports machine 0xfd1d
# (IMAGE_FILE_MACHINE_AMD64 ^ 0x7b79) and its CLR header points at a native header beginning
# with the ReadyToRun signature.
#
# Requires: the .NET SDK. Built with 9.0.316.
set -e

OUT=$(cd "$(dirname "$0")/../../tests" && pwd)
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

cd "$WORK"
dotnet new classlib -o readytorun --framework net9.0
cat > readytorun/Class1.cs <<'EOF'
namespace ReadyToRunSample;

public static class Arithmetic
{
    public static int Accumulate(int[] values)
    {
        int total = 0;
        for (int i = 0; i < values.Length; i++)
        {
            total = total * 31 + values[i];
        }
        return total;
    }

    public static int Fibonacci(int n) => n < 2 ? n : Fibonacci(n - 1) + Fibonacci(n - 2);
}
EOF

cd readytorun
# DebugType=none keeps the build directory out of the image's debug directory.
dotnet publish -c Release -r linux-x64 --self-contained false -p:PublishReadyToRun=true -p:DebugType=none
cp bin/Release/net9.0/linux-x64/publish/readytorun.dll "$OUT/x86_64/readytorun_linux_x64.dll"
