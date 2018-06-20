/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class MixedJava */

#ifndef _Included_MixedJava
#define _Included_MixedJava
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     MixedJava
 * Method:    sum_array
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_MixedJava_sum_1array
  (JNIEnv *, jclass, jintArray);

/*
 * Class:     MixedJava
 * Method:    get_array_length_native
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_MixedJava_get_1array_1length_1native
  (JNIEnv *, jclass, jintArray);

/*
 * Class:     MixedJava
 * Method:    boolean_arr
 * Signature: ([ZI)Z
 */
JNIEXPORT jboolean JNICALL Java_MixedJava_boolean_1arr
  (JNIEnv *, jclass, jbooleanArray, jint);

/*
 * Class:     MixedJava
 * Method:    byte_arr
 * Signature: ([BI)B
 */
JNIEXPORT jbyte JNICALL Java_MixedJava_byte_1arr
  (JNIEnv *, jclass, jbyteArray, jint);

/*
 * Class:     MixedJava
 * Method:    char_arr
 * Signature: ([CI)C
 */
JNIEXPORT jchar JNICALL Java_MixedJava_char_1arr
  (JNIEnv *, jclass, jcharArray, jint);

/*
 * Class:     MixedJava
 * Method:    short_arr
 * Signature: ([SI)S
 */
JNIEXPORT jshort JNICALL Java_MixedJava_short_1arr
  (JNIEnv *, jclass, jshortArray, jint);

/*
 * Class:     MixedJava
 * Method:    int_arr
 * Signature: ([II)I
 */
JNIEXPORT jint JNICALL Java_MixedJava_int_1arr
  (JNIEnv *, jclass, jintArray, jint);

/*
 * Class:     MixedJava
 * Method:    long_arr
 * Signature: ([JI)J
 */
JNIEXPORT jlong JNICALL Java_MixedJava_long_1arr
  (JNIEnv *, jclass, jlongArray, jint);

/*
 * Class:     MixedJava
 * Method:    arr_symbolic
 * Signature: ([II)I
 */
JNIEXPORT jint JNICALL Java_MixedJava_arr_1symbolic
  (JNIEnv *, jclass, jintArray, jint);

/*
 * Class:     MixedJava
 * Method:    modify_int_arr
 * Signature: ([I)I
 */
JNIEXPORT jint JNICALL Java_MixedJava_modify_1int_1arr
  (JNIEnv *, jclass, jintArray);

/*
 * Class:     MixedJava
 * Method:    int_arr_region
 * Signature: ([III)I
 */
JNIEXPORT jint JNICALL Java_MixedJava_int_1arr_1region
  (JNIEnv *, jclass, jintArray, jint, jint);

/*
 * Class:     MixedJava
 * Method:    modify_int_arr_region
 * Signature: ([III)V
 */
JNIEXPORT void JNICALL Java_MixedJava_modify_1int_1arr_1region
  (JNIEnv *, jclass, jintArray, jint, jint);

/*
 * Class:     MixedJava
 * Method:    new_int_array
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_MixedJava_new_1int_1array
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif