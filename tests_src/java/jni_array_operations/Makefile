DEBUGFLAGS = -g3 -ggdb3 -O0

INCLUDES = -I /usr/lib/jvm/java-8-openjdk-amd64/include/linux \
 		   -I /usr/lib/jvm/java-8-openjdk-amd64/include/ 

currentdir = `basename $(CURDIR)`
binaries_dir = ../../../tests/java/$(currentdir)/

all : jar header lib 
	
jar : ./src/mixedjava.java
	javac -classpath . -d . ./src/mixedjava.java
	#        file          entry     source
	jar -cfe mixedjava.jar MixedJava *.class
	rm *.class
	
header : ./src/mixedjava.c
	javac -classpath . -d . ./src/mixedjava.java
	javah -classpath . -o ./src/mixedjava.h MixedJava
	rm *.class
	
lib : ./src/mixedjava.c header
	gcc $(DEBUGFLAGS) -fPIC $(INCLUDES) -c ./src/mixedjava.c -o mixedjava.o
	gcc $(DEBUGFLAGS) -W -shared -o libmixedjava.so mixedjava.o
	rm mixedjava.o
	
test :
	java -Xint -Djava.library.path=. -jar MixedJava.jar

# move libs from binaries/test_src/java/<test_dir> 
#             to binaries/tests/java/<test_dir>
move :	
	rm -rf $(binaries_dir) && mkdir $(binaries_dir)
	mv libmixedjava.so $(binaries_dir) && mv mixedjava.jar $(binaries_dir)
	
clean :
	rm -f mixedjava.jar libmixedjava.so mixedjava.o *.class ./src/mixedjava.h
