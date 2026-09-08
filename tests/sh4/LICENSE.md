The file `hello_32x_sh2be` is the Sega 32X "hello world" skeleton example from
[marsdev](https://github.com/andwn/marsdev), built from `examples/32x-skeleton`
at revision `3318d3f39823154b24ce48bfa5d8fe6e3f6cde3f` with the SuperH GCC
13.1.0 toolchain marsdev ships. It is a big-endian SH-2 ELF executable, which
angr loads through pypcode as `SuperH4:BE:32:default`.

marsdev has the following license:

```
MIT License

Copyright (c) 2023 Andrew DeRosier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
