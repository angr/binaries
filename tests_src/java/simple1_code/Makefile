# simple1.jar is built by ../build_test_jars.py with a Java 8 compiler. This
# target compiles the same sources to a newer class file version, giving
# pysoot's class file version check an input that Soot cannot parse.

binaries_dir = ../../../tests/java

all : simple1_java17.jar

simple1_java17.jar : src/simple1/Class1.java src/simple1/Class2.java
	rm -rf classes && mkdir classes
	javac --release 17 -d classes $^
	jar --create --file $@ -C classes .
	rm -rf classes

move : simple1_java17.jar
	mv simple1_java17.jar $(binaries_dir)/

clean :
	rm -rf classes simple1_java17.jar
