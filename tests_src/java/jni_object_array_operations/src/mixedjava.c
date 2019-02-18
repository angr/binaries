#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "mixedjava.h"

/*
 * Class:     MixedJava
 * Method:    set_object_array_elem
 * Signature: ([LMixedJava;LMixedJava;I)V
 */
JNIEXPORT void JNICALL
Java_MixedJava_set_1object_1array_1elem(JNIEnv *env, jclass cls, jobjectArray arr, jobject obj, jint idx){
	(*env)->SetObjectArrayElement(env, arr, idx, obj);
}

/*
 * Class:     MixedJava
 * Method:    get_object_array_elem
 * Signature: ([LMixedJava;I)LMixedJava;
 */
JNIEXPORT jobject JNICALL
Java_MixedJava_get_1object_1array_1elem(JNIEnv *env, jclass cls, jobjectArray arr, jint idx){
	return (*env)->GetObjectArrayElement(env, arr, idx);
}

/*
 * Class:     MixedJava
 * Method:    create_new_object_array
 * Signature: (LMixedJava;)[LMixedJava;
 */
JNIEXPORT jobjectArray JNICALL
Java_MixedJava_create_1new_1object_1array(JNIEnv *env, jclass cls, jobject obj){
	return (*env)->NewObjectArray(env, 5, cls, obj);
}