DEBUGFLAGS = -g3 -ggdb3 -O0

INCLUDES = -I /usr/lib/jvm/java-8-openjdk-amd64/include/linux \
           -I /usr/lib/jvm/java-8-openjdk-amd64/include/

currentdir = `basename $(CURDIR)`
binaries_dir = ../../../tests/java/$(currentdir)/

all : jar

jar : ./src/mixedjava.java lib
	javac -classpath '.:mylib.jar' -d . ./src/mixedjava.java
	jar -cfe mixedjava.jar MixedJava *.class
	rm *.class

lib : ./src/MyLib.java
	javac -classpath . -d . ./src/MyLib.java
	jar -cfe mylib.jar MyLib ./mylib/*.class
	rm ./mylib/*.class

move :
	rm -rf $(binaries_dir) && mkdir $(binaries_dir)
	mv mixedjava.jar $(binaries_dir)
	rm -f mylib.jar
	rm -rf mylib

clean :
	rm -f mixedjava.jar mylib.jar *.class
	rm -rf mylib
