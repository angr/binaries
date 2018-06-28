#include <jni.h>
#include <stdlib.h>
#include "mixedjava.h"

JNIEXPORT jint JNICALL 
Java_MixedJava_sum_1array(JNIEnv *env, jclass cls, jintArray arr){
	jsize len = (*env)->GetArrayLength(env, arr);
	int i, sum = 0;
	jint *body = (*env)->GetIntArrayElements(env, arr, 0);
	for (i=0; i<len; i++) {
		sum += body[i];
	}
	(*env)->ReleaseIntArrayElements(env, arr, body, 0);
	return sum;
}


JNIEXPORT jint JNICALL 
Java_MixedJava_get_1array_1length_1native(JNIEnv *env, jclass cls, jintArray arr){
	jsize len = (*env)->GetArrayLength(env, arr);
	return len;
}

JNIEXPORT jint JNICALL 
Java_MixedJava_return_1element(JNIEnv *env, jclass cls, jintArray arr, jint idx){
	jboolean isCopy = 5;
	jint *body = (*env)->GetIntArrayElements(env, arr, &isCopy);
	if (isCopy == JNI_TRUE){
		return body[1]; 
	}
	return body[3];
}

JNIEXPORT jboolean JNICALL 
Java_MixedJava_boolean_1arr(JNIEnv *env, jclass cls, jbooleanArray arr, jint idx){	
	jboolean isCopy = 5;
	jboolean *body = (*env)->GetBooleanArrayElements(env, arr, &isCopy);
	if (isCopy == JNI_TRUE){
		jboolean elem = body[idx];
		body[idx-1] = JNI_FALSE;
		(*env)->ReleaseBooleanArrayElements(env, arr, body, 0);
		return elem + 1;
	}
}

JNIEXPORT jbyte JNICALL
Java_MixedJava_byte_1arr(JNIEnv *env, jclass cls, jbyteArray arr, jint idx){
	jboolean isCopy = 5;
	jbyte *body = (*env)->GetByteArrayElements(env, arr, &isCopy);
	if (isCopy == JNI_TRUE){
		jbyte elem = body[idx];
		body[idx-1] = elem + 1;
		(*env)->ReleaseByteArrayElements(env, arr, body, 0);
		return elem + 1;
	}

}
		
JNIEXPORT jchar JNICALL
Java_MixedJava_char_1arr(JNIEnv *env, jclass cls, jcharArray arr, jint idx){
	jboolean isCopy = 5;
	jchar *body = (*env)->GetCharArrayElements(env, arr, &isCopy);
	if (isCopy == JNI_TRUE){
		jchar elem = body[idx];
		body[idx-1] = elem + 1;
		(*env)->ReleaseCharArrayElements(env, arr, body, 0);
		return elem + 1;
	}
}

JNIEXPORT jshort JNICALL
Java_MixedJava_short_1arr(JNIEnv *env, jclass cls, jshortArray arr, jint idx){
	jboolean isCopy = 5;
	jshort *body = (*env)->GetShortArrayElements(env, arr, &isCopy);
	if (isCopy == JNI_TRUE){
		jshort elem = body[idx];
		body[idx-1] = elem + 1;
		(*env)->ReleaseShortArrayElements(env, arr, body, 0);
		return elem + 1;
	}
}

JNIEXPORT jint JNICALL
Java_MixedJava_int_1arr(JNIEnv *env, jclass cls, jintArray arr, jint idx){
	jboolean isCopy = 5;
	jint *body = (*env)->GetIntArrayElements(env, arr, &isCopy);
	if (isCopy == JNI_TRUE){
		jint elem = body[idx];
		body[idx-1] = elem + 1;
		(*env)->ReleaseIntArrayElements(env, arr, body, 0);
		return elem + 1;
	}

}
					
JNIEXPORT jlong JNICALL
Java_MixedJava_long_1arr(JNIEnv *env, jclass cls, jlongArray arr, jint idx){
	jboolean isCopy = 5;
	jlong *body = (*env)->GetLongArrayElements(env, arr, &isCopy);
	if (isCopy == JNI_TRUE){
		jlong elem = body[idx];
		body[idx-1] = elem + 1;
		(*env)->ReleaseLongArrayElements(env, arr, body, 0);
		return elem + 1;
	}
}

JNIEXPORT jint JNICALL 
Java_MixedJava_1arr_1symbolic(JNIEnv *env, jclass cls, jintArray arr, jint idx){
	jint *body = (*env)->GetIntArrayElements(env, arr, 0);
	if (idx == 223)
		return body[idx];
	else
		return 0;
}

JNIEXPORT jint JNICALL 
Java_MixedJava_int_1arr_1region(JNIEnv *env, jclass cls, jintArray arr, jint start_idx, jint length){
	jint *buf = (jint*) malloc(length * sizeof(jint));
	(*env)->GetIntArrayRegion(env, arr, start_idx, length, buf);
	jint result = 0;
	for (int i=0; i < length; i++)
		result += buf[i];
	return result;
}

JNIEXPORT jint JNICALL 
Java_MixedJava_modify_1int_1arr(JNIEnv *env, jclass cls, jintArray arr){
	jboolean isCopy = JNI_FALSE;
	jint *body = (*env)->GetIntArrayElements(env, arr, &isCopy);
	body[0] = 4;
	body[1] = 3;
	body[3] = 1;
	body[4] = 0;
	if (isCopy == JNI_TRUE)
		(*env)->ReleaseIntArrayElements(env, arr, body, 0);	
}

JNIEXPORT void JNICALL
Java_MixedJava_modify_1int_1arr_1region(JNIEnv *env, jclass cls, jintArray arr, jint start_idx, jint length){
	jint *buf = (jint*) malloc(3 * sizeof(jint));
	(*env)->GetIntArrayRegion(env, arr, start_idx, length, buf);
	
	jint tmp = buf[0];
	buf[0] = buf[2];
	buf[2] = tmp;
	
	(*env)->SetIntArrayRegion(env, arr, start_idx, length, buf);
}

JNIEXPORT jintArray JNICALL
Java_MixedJava_new_1int_1array(JNIEnv *env, jclass cls){
	int arr[] = {0, 1, 2, 3, 4};
	jintArray java_arr = (*env)->NewIntArray(env, 5);
	(*env)->SetIntArrayRegion(env, java_arr, 0, 5, arr);
	return java_arr;
}