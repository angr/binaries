The file `multiarch_main_main.o` is part of valgrind, and has the following license:

```
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2004-2015 OpenWorks LLP
      info@open-works.net

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

   The GNU General Public License is contained in the file COPYING.

   Neither the names of the U.S. Department of Energy nor the
   University of California nor the names of its contributors may be
   used to endorse or promote products derived from this software
   without prior written permission.
```

The files `cat`, `true`, and `decompiler/coreutils_sum_O2` are from `coreutils`, and have the following license:

```
Copyright (C) 2017 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
```

The file `decompiler/coreutils_sum_O2` is the unchanged GNU/Linux `sum` artifact
from pinned DecBench revision `4b42a0dc6158913db0648a9123e76d6ddd9ab9cf`:

<https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/4b42a0dc6158913db0648a9123e76d6ddd9ab9cf/binaries/O2/coreutils/sum>

Its SHA-256 is
`2d700fcb7e47688324231eb3463284a4b8dcf7579df746ddb1ecceb24723b33e`.

The file `decompiler/openssh_scp_O2_noinline` is GNU/Linux OpenSSH portable
`scp`, derived from DecBench revision
`4b42a0dc6158913db0648a9123e76d6ddd9ab9cf` by removing debug sections with
GNU `objcopy --strip-debug`. The public source artifact is pinned at
<https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/4b42a0dc6158913db0648a9123e76d6ddd9ab9cf/binaries/O2-noinline/openssh-portable/scp>.
The source artifact has SHA-256
`102cbce7585c1662dfa7dfd35344ab2d6da5d08ce415f9db6cb165e70db21fa3`; the
committed derivative has SHA-256
`70475811049560f671bf9df207f99a2ff8ebc4c0c4fa69a0915d5082c515ff57`.
OpenSSH portable is distributed under the copyright notices and permissive
licenses reproduced in `decompiler/openssh-portable-LICENCE`. The original is
pinned at:

<https://github.com/openssh/openssh-portable/blob/0ffb46f2ee2ffcc4daf45ee679e484da8fcf338c/LICENCE>
