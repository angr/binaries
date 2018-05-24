DEBUGFLAGS = -g3 -ggdb3 -O0

INCLUDES = -I /usr/lib/jvm/java-8-openjdk-amd64/include/linux \
 		   -I /usr/lib/jvm/java-8-openjdk-amd64/include/ 

all : jar header lib 
	
jar : ./src/mixedjava.java
	javac -classpath . -d . ./src/mixedjava.java
	#        file          entry     source
	jar -cfe MixedJava.jar MixedJava MixedJava.class
	rm MixedJava.class
	
header : ./src/mixedjava.c
	javac -classpath . -d . ./src/mixedjava.java
	javah -classpath . -o ./src/mixedjava.h MixedJava
	rm MixedJava.class
	
lib : ./src/mixedjava.c header
	gcc $(DEBUGFLAGS) -fPIC $(INCLUDES) -c ./src/MixedJava.c -o MixedJava.o
	gcc $(DEBUGFLAGS) -W -shared -o libmixedjava.so MixedJava.o
	rm MixedJava.o
	
test :
	java -Xint -Djava.library.path=. -jar MixedJava.jar
	
clean :
	rm -f MixedJava.jar libmixedjava.so MixedJava.o MixedJava.class ./src/mixedjava.h