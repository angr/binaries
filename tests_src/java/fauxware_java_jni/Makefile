DEBUGFLAGS = -g3 -ggdb3 -O0

INCLUDES = -I /usr/lib/jvm/java-8-openjdk-amd64/include/linux \
 		   -I /usr/lib/jvm/java-8-openjdk-amd64/include/ 

currentdir = `basename $(CURDIR)`
binaries_dir = ../../../tests/java/$(currentdir)/

all : jar header lib

jar : ./src/fauxware_jni.java
	javac -classpath . -d . ./src/fauxware_jni.java
	#        file          entry     source
	jar -cfe fauxware.jar Fauxware *.class
	rm *.class
	
header : ./src/fauxware_jni.c
	javac -classpath . -d . ./src/fauxware_jni.java
	javah -classpath . -o ./src/fauxware_jni.h Fauxware
	rm *.class
	
lib : ./src/fauxware_jni.c header
	gcc $(DEBUGFLAGS) -fPIC $(INCLUDES) -c ./src/fauxware_jni.c -o fauxware_jni.o
	gcc $(DEBUGFLAGS) -W -shared -o libfauxware.so fauxware_jni.o
	rm fauxware_jni.o
	
test :
	java -Xint -Djava.library.path=. -jar fauxware.jar SOSNEAKY

# move libs from binaries/test_src/java/<test_dir> 
#             to binaries/tests/java/<test_dir>
move :	
	rm -rf $(binaries_dir) && mkdir $(binaries_dir)
	mv libfauxware.so $(binaries_dir) && mv fauxware.jar $(binaries_dir)

clean :
	rm -f fauxware.jar libfauxware.so fauxware_jni.o *.class ./src/fauxware_jni.h
