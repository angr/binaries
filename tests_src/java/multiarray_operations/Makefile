DEBUGFLAGS = -g3 -ggdb3 -O0

INCLUDES = -I /usr/lib/jvm/java-8-openjdk-amd64/include/linux \
           -I /usr/lib/jvm/java-8-openjdk-amd64/include/

currentdir = `basename $(CURDIR)`
binaries_dir = ../../../tests/java/$(currentdir)/

all : jar

jar : ./src/mixedjava.java
	javac -classpath . -d . ./src/mixedjava.java
	#        file          entry     source
	jar -cfe mixedjava.jar MixedJava *.class
	rm *.class

test :
	java -Xint -Djava.library.path=. -jar Main.jar

# move libs from binaries/test_src/java/<test_dir>
#             to binaries/tests/java/<test_dir>
move :
	rm -rf $(binaries_dir) && mkdir $(binaries_dir)
	mv mixedjava.jar $(binaries_dir)

clean :
	rm -f mixedjava.jar libmixedjava.so mixedjava.o *.class ./src/mixedjava.h

