#include <jni.h>
#include <stdio.h>
#include "mixedjava.h"

JNIEXPORT jbyte JNICALL Java_MixedJava_native_1byte(JNIEnv *env, jclass cls, jbyte b0){
	return b0 + b0;
}

JNIEXPORT jshort JNICALL Java_MixedJava_native_1short(JNIEnv *env, jclass cls, jshort s0){
	return s0 >> 2;		
}

JNIEXPORT jchar JNICALL Java_MixedJava_native_1char(JNIEnv *env, jclass cls, jchar c0){
	return c0 + 1;
}

JNIEXPORT jboolean JNICALL Java_MixedJava_native_1boolean(JNIEnv *env, jclass cls, jboolean b0){
	if (b0){
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}	
}

JNIEXPORT jlong JNICALL Java_MixedJava_native_1long(JNIEnv *env, jclass cls, jlong l0){
	return l0 + 2;
}

JNIEXPORT jint JNICALL Java_MixedJava_native_1int(JNIEnv *env, jclass cls, jint i0){
	return -i0;
}