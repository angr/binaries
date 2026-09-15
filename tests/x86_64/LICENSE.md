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

The files `cat` and `true` are from `coreutils`, and have the following license:

```
Copyright (C) 2017 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
```

The `paste` binary is from GNU coreutils 9.1 and has the following license:

```
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
```

It was derived from DecBench revision
`4b42a0dc6158913db0648a9123e76d6ddd9ab9cf` at
`binaries/O2-noinline/coreutils/paste`, then stripped of debug sections with
`objcopy --strip-debug`. The source binary's SHA-256 digest is
`9c32fa84a1260d50224c008180ababd41f47c8ba8e7d021671c6f6e93d964d92`;
the resulting fixture's SHA-256 digest is
`01fcdf629c4994d0fe733c10a92dc142909be74c9d8c9efe04b703ae6cbf103d`.
