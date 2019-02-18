#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "mixedjava.h"

/*
 * Class:     MixedJava
 * Method:    concat_string
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_MixedJava_concat_1string(JNIEnv *env, jclass cls, jstring jstr1, jstring jstr2){
	const char* str1 = (*env)->GetStringUTFChars(env, jstr1, 0);
	const char* str2 = (*env)->GetStringUTFChars(env, jstr2, 0);
	
	char *out_str = "";
	strcat(out_str, str1);
	strcat(out_str, str2);
	strcat(out_str, "!");
	
	(*env)->ReleaseStringUTFChars(env, jstr1, str1);
	(*env)->ReleaseStringUTFChars(env, jstr2, str2);
	
	return (*env)->NewStringUTF(env, out_str);
	
}

/*
 * Class:     MixedJava
 * Method:    strlen
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_MixedJava_strlen(JNIEnv *env, jclass cls, jstring str){
	return (*env)->GetStringUTFLength(env, str);
}