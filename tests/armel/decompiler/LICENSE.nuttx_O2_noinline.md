# NuttX O2-noinline fixture provenance

`nuttx_O2_noinline` is a debug-sanitized derivative of the NuttX 12.13.0
`stm32f4discovery:nsh` firmware artifact from pinned DecBench dataset revision
`e5eb576d66ee36793b800a4dd45e291e0add4472`:

<https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/e5eb576d66ee36793b800a4dd45e291e0add4472/binaries/O2-noinline/nuttx/nuttx>

The source artifact is 2,301,460 bytes and has SHA-256
`21db6ee71199fd88c61978bd1d54d80a129741d5417f3d95a5f669dcd4f44910`.
The committed derivative was produced with GNU Binutils 2.46 using:

```text
arm-none-eabi-objcopy --remove-section=.debug_* nuttx nuttx_O2_noinline
```

The derivative is 175,804 bytes, mode 0755, and has SHA-256
`d30ea99781f9d0aa0d4db54d50a81b73adc5514d659c7059e55c54fd4a4c2bfb`.
The source artifact identifies its NuttX source revision as
`b9b5d9afbf5a88f3c52830d4ce88ae038be0fd35`:

<https://github.com/apache/nuttx/commit/b9b5d9afbf5a88f3c52830d4ce88ae038be0fd35>

NuttX is distributed under the Apache License 2.0. The license at that exact
source revision is available at:

<https://github.com/apache/nuttx/blob/b9b5d9afbf5a88f3c52830d4ce88ae038be0fd35/LICENSE>
