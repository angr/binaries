#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "mixedjava.h"

jobject global_ref;

/*
 * Class:     MixedJava
 * Method:    create_global_ref
 * Signature: (LMixedJava;)V
 */
JNIEXPORT void JNICALL
Java_MixedJava_create_1global_1ref(JNIEnv *env, jclass cls, jobject obj){
	global_ref = (*env)->NewGlobalRef(env, obj);
}

/*
 * Class:     MixedJava
 * Method:    delete_global_ref
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_MixedJava_delete_1global_1ref(JNIEnv *env, jclass cls){
	(*env)->DeleteGlobalRef(env, global_ref);
}

/*
 * Class:     MixedJava
 * Method:    access_global_ref
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_MixedJava_access_1global_1ref(JNIEnv *env, jclass cls){
	jclass obj_class = (*env)->GetObjectClass(env, global_ref);
	jfieldID obj_field = (*env)->GetFieldID(env, obj_class, "i", "I");
	jint value = (*env)->GetIntField(env, global_ref, obj_field);
	return value;
}
