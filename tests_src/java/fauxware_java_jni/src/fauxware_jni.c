#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "fauxware_jni.h"

char *sneaky = "SOSNEAKY";

int authenticate(const char *password){
	char stored_pw[9];
	stored_pw[8] = 0;
	int pwfile;
	// evil back d00r
	if (strcmp(password, sneaky) == 0) return 1;
    // read admin password
	pwfile = open("pwfile", O_RDONLY);
	read(pwfile, stored_pw, 8);
    // check password
	if (strcmp(password, stored_pw) == 0) return 1;
	return 0;
}

JNIEXPORT jboolean JNICALL
Java_Fauxware_authenticate (JNIEnv *env, jclass cls, jstring password_ref){
	const char *password = (*env)->GetStringUTFChars(env, password_ref, 0);
    jboolean authed = authenticate(password);
	(*env)->ReleaseStringUTFChars(env, password_ref, password);
	return authed;
}
