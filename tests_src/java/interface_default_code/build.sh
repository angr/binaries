#!/bin/bash
# Builds tests/java/interface_default.jar.
#
# Impl.greet() calls Greeter.super.greet(), which compiles to invokespecial whose
# declaring class is an interface. Targeting 8 keeps the class file within what the
# Soot bundled with pysoot understands.
#
# Built with openjdk 17 from nixpkgs:
#     nix shell nixpkgs#jdk17 --command ./build.sh
set -ex

BASE_DIR=$(dirname "$0")
OUTPUT_DIR=$BASE_DIR/../../../tests/java

rm -rf "$BASE_DIR/classes"
javac -source 8 -target 8 -d "$BASE_DIR/classes" "$BASE_DIR"/src/iface/*.java
jar --create --file "$OUTPUT_DIR/interface_default.jar" --main-class iface.Impl -C "$BASE_DIR/classes" .
rm -rf "$BASE_DIR/classes"
