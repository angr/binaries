#include <jni.h>
#include "mixedjava.h"

JNIEXPORT jint JNICALL 
Java_MixedJava_get_1version(JNIEnv *env, jclass cls){
	jint version = (*env)->GetVersion(env);
	return version;
}