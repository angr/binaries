#include <jni.h>
#include <stdlib.h>
#include "mixedjava.h"

//
// Instance Method Calls
//

/*
 * Class:     MixedJava
 * Method:    native_callback
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_MixedJava_native_1callback(JNIEnv *env, jobject obj){
	jclass obj_class = (*env)->GetObjectClass(env, obj);
	jmethodID mid = (*env)->GetMethodID(env, obj_class, "callback", "(I)I");
	jint val = (*env)->CallIntMethod(env, obj, mid, 7);
	return val;
}

/*
 * Class:     MixedJava
 * Method:    native_callbackA
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_MixedJava_native_1callbackA(JNIEnv *env, jobject obj){
	jclass obj_class = (*env)->GetObjectClass(env, obj);
	jmethodID mid = (*env)->GetMethodID(env, obj_class, "callbackA", "(ILMixedJava;)V");
	jvalue* args = (jvalue*) malloc(2 * sizeof(jvalue));
	args[0] = (jvalue) 1;
	args[1] = (jvalue) obj;
	(*env)->CallVoidMethodA(env, obj, mid, args);
}

/*
 * Class:     MixedJava
 * Method:    native_callback_2
 * Signature: (LMixedJava;LMixedJava;)V
 */
JNIEXPORT void JNICALL 
Java_MixedJava_native_1callback_12(JNIEnv *env, jclass cls, jobject obj1, jobject obj2){
	jmethodID mid = (*env)->GetMethodID(env, cls, "callback", "(I)I");
	(*env)->CallVoidMethod(env, obj1, mid, 8);
	(*env)->CallVoidMethod(env, obj2, mid, 9);
}

//
// Non-Virtual Instance Method Calls
//

/*
 * Class:     MixedJava
 * Method:    native_non_virtual_callback
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_MixedJava_native_1non_1virtual_1callback(JNIEnv *env, jobject obj){
	jclass obj_class = (*env)->GetObjectClass(env, obj);
	jclass super_cls = (*env)->GetSuperclass(env, obj_class);
	jmethodID mid;
	//mid = (*env)->GetMethodID(env, obj_class, "callback", "(I)I"); // result is 2
	mid = (*env)->GetMethodID(env, super_cls, "callback", "(I)I"); // result is 5
	jint val = (*env)->CallNonvirtualIntMethod(env, obj, super_cls, mid, 5);
	return val;
}

//
// Static Method Calls
//

/*
 * Class:     MixedJava
 * Method:    native_static_callback
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_MixedJava_native_1static_1callback(JNIEnv *env, jclass cls){
	jmethodID mid = (*env) -> GetStaticMethodID(env, cls, "static_callback", "(I)I");
	jint val = (*env) -> CallStaticIntMethod(env, cls, mid, 5);
}

/*
 * Class:     MixedJava
 * Method:    native_static_callback_2
 * Signature: ()LMixedJava;
 */
JNIEXPORT jobject JNICALL
Java_MixedJava_native_1static_1callback_12(JNIEnv *env, jclass cls){
	jmethodID mid = (*env) -> GetStaticMethodID(env, cls, "static_callback2", "()LMixedJava;");
	jobject obj = (*env) -> CallStaticObjectMethod(env, cls, mid);
	return obj;
}


