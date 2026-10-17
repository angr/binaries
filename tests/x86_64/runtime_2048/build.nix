let
  pkgs = import (builtins.fetchTree {
    type = "github";
    owner = "NixOS";
    repo = "nixpkgs";
    rev = "6b5e5b7a6631f065bf6908986990b37d845f847f";
    narHash = "sha256-smTKQXMLLStzc8zJevMCckbk3My7SvbbLmPYZUJJKW4=";
  }) { system = "x86_64-linux"; };
in
pkgs.stdenv.mkDerivation {
  pname = "2048-runtime-fixtures";
  version = "90343eb209e84fa9687d596f5d5eecfdf7d4df51";
  dontUnpack = true;
  nativeBuildInputs = [ pkgs.patchelf ];
  dontStrip = true;
  dontFixup = true;
  buildPhase = ''
    # Keep Nix's temporary output name out of DWARF and the ELF build ID.
    export NIX_CFLAGS_COMPILE="-frandom-seed=runtime-2048"
    export NIX_LDFLAGS=""
    cp ${./2048.c} 2048.c
    $CC -std=c99 -O0 -g -fno-omit-frame-pointer -fPIE -pie \
      -ffile-prefix-map="$PWD"=. -Wl,--build-id=sha1 \
      2048.c -o 2048.pie
    $CC -std=c99 -O0 -g -fno-omit-frame-pointer -fno-PIE -no-pie \
      -ffile-prefix-map="$PWD"=. -Wl,--build-id=sha1 \
      2048.c -o 2048.nonpie
    for game in 2048.pie 2048.nonpie; do
      patchelf --set-interpreter /lib64/ld-linux-x86-64.so.2 --remove-rpath "$game"
    done
    cp 2048.pie 2048.stripped
    $STRIP --strip-all 2048.stripped
  '';
  installPhase = ''
    mkdir -p $out
    cp 2048.pie 2048.nonpie 2048.stripped $out/
  '';
}
