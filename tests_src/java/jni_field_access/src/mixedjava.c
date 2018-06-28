#include <jni.h>
#include <stdlib.h>
#include "mixedjava.h"

//
// Instance Field Access
//


/*
 * Class:     MixedJava
 * Method:    set_field_f1
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_MixedJava_set_1field_1f1 (JNIEnv *env, jobject obj, jint val){
	jclass obj_class = (*env)->GetObjectClass(env, obj);
	jfieldID obj_field1 = (*env)->GetFieldID(env, obj_class, "f1", "I");
	(*env)->SetIntField(env, obj, obj_field1, val);
}

/*
 * Class:     MixedJava
 * Method:    get_field_f1
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_MixedJava_get_1field_1f1(JNIEnv *env, jobject obj){
	jclass obj_class = (*env)->GetObjectClass(env, obj);
	jfieldID obj_field1 = (*env)->GetFieldID(env, obj_class, "f1", "I");
	jint value = (*env)->GetIntField(env, obj, obj_field1);
	return value;
}

/*
 * Class:     MixedJava
 * Method:    get_field_f2
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_MixedJava_get_1field_1f2(JNIEnv *env, jobject obj){
	jclass obj_class = (*env)->GetObjectClass(env, obj);
	jfieldID obj_field1 = (*env)->GetFieldID(env, obj_class, "f2", "I");
	jint value = (*env)->GetIntField(env, obj, obj_field1);
	return value;
}

// 
// Static Field Access
//

/*
 * Class:     MixedJava
 * Method:    get_static_field
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_MixedJava_get_1static_1field(JNIEnv *env, jclass cls){
	jfieldID field_id = (*env)->GetStaticFieldID(env, cls, "s1", "I");
	jint value = (*env)->GetStaticIntField(env, cls, field_id);
	return value;
}

/*
 * Class:     MixedJava
 * Method:    set_static_field
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_MixedJava_set_1static_1field(JNIEnv *env, jclass cls, jint val){
	jfieldID field_id = (*env)->GetStaticFieldID(env, cls, "s2", "I");
	(*env)->SetStaticIntField(env, cls, field_id, val);
}

/*
 * Class:     MixedJava
 * Method:    get_static_field_access_subclass
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_MixedJava_get_1static_1field_1access_1subclass(JNIEnv *env, jclass cls){
	jclass sub_cls = (*env)->FindClass(env, "SubMixedJava");
	
	jfieldID field_id = (*env)->GetStaticFieldID(env, sub_cls, "s2", "I");
	jint value = (*env)->GetStaticIntField(env, sub_cls, field_id);
	
	jfieldID field_id_2 = (*env)->GetStaticFieldID(env, sub_cls, "s1", "I");
	(*env)->SetStaticIntField(env, sub_cls, field_id_2, 10);
	
	jfieldID field_id_3 = (*env)->GetStaticFieldID(env, sub_cls, "s3", "I");
	(*env)->SetStaticIntField(env, sub_cls, field_id_3, 30);
	return value;
}
