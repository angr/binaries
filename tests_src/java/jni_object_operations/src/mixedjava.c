#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "mixedjava.h"

/*
 * Class:     MixedJava
 * Method:    alloc_object
 * Signature: ()LMixedJava;
 */
JNIEXPORT jobject JNICALL 
Java_MixedJava_alloc_1object(JNIEnv *env, jclass cls){
	jobject obj = (*env)->AllocObject(env, cls);
	return obj;
}

/*
 * Class:     MixedJava
 * Method:    new_object
 * Signature: ()LMixedJava;
 */
JNIEXPORT jobject JNICALL
Java_MixedJava_new_1object(JNIEnv *env, jclass cls){
	jmethodID mid = (*env)->GetMethodID(env, cls, "<init>", "(I)V");
	jobject obj = (*env)->NewObject(env, cls, mid, 1);
	return obj;
}

/*
 * Class:     MixedJava
 * Method:    new_subclass_object
 * Signature: ()LSubMixedJava;
 */
JNIEXPORT jobject JNICALL 
Java_MixedJava_new_1subclass_1object(JNIEnv *env, jclass cls){
	jclass sub_class = (*env)->FindClass(env, "SubMixedJava");
	jmethodID mid = (*env)->GetMethodID(env, sub_class, "<init>", "(I)V");
	jobject obj = (*env)->NewObject(env, sub_class, mid, 1);
	return obj;
}

/*
 * Class:     MixedJava
 * Method:    isinstanceof
 * Signature: (LMixedJava;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL 
Java_MixedJava_isinstanceof(JNIEnv *env, jclass cls, jobject obj, jstring jcls_name){
	const char* cls_name = (*env)->GetStringUTFChars(env, jcls_name, 0);	
	jclass target_cls = (*env)->FindClass(env, cls_name);
	return (*env)->IsInstanceOf(env, obj, target_cls);
}

/*
 * Class:     MixedJava
 * Method:    issameobject
 * Signature: (LMixedJava;LMixedJava;)I
 */
JNIEXPORT jint JNICALL
Java_MixedJava_issameobject(JNIEnv *env, jclass cls, jobject obj1, jobject obj2){
	return (*env)->IsSameObject(env, obj1, obj2);
}