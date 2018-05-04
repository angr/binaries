
#include "../jni/NotFun.h"

#include <jni.h>
#include <stdio.h>

JNIEXPORT long JNICALL Java_NotFun_getUnicornColor(JNIEnv *env, jobject thisObj,jlong address) {
   return *(long*)address;
}

JNIEXPORT void JNICALL Java_NotFun_kill(JNIEnv *env, jobject thisObj,jlong address, jint bb) {
   unsigned long long v = *(long*)address;
   v ^= (1<<bb);
   *(long*)address = v;
}

JNIEXPORT long JNICALL Java_NotFun_string(JNIEnv *env, jobject thisObj, jstring s) {
   printf("ss: %p\n",(long**)(&s));
   fflush(stdout);
   return **(long**)(&s);
}

JNIEXPORT int JNICALL Java_NotFun_mult(JNIEnv *env, jobject thisObj,jint n1, jint n2) {
   return n1*n2;
}

JNIEXPORT long JNICALL Java_NotFun_getUnicornLocation(JNIEnv *env, jobject thisObj, jstring s) {
   return **(long**)(&s);
}

JNIEXPORT int JNICALL Java_NotFun_magic000(JNIEnv *env, jobject thisObj, jint n) {
   return n*3 + 1;
}

JNIEXPORT int JNICALL Java_NotFun_magic0(JNIEnv *env, jobject thisObj, jint n) {
   return (n<<1) + 4;
}
JNIEXPORT int JNICALL Java_NotFun_magic1(JNIEnv *env, jobject thisObj, jint n) {
   return (n<<4) + 3;
}
JNIEXPORT int JNICALL Java_NotFun_magic2(JNIEnv *env, jobject thisObj, jint n) {
   return (n<<3) + 2;
}
JNIEXPORT int JNICALL Java_NotFun_magic3(JNIEnv *env, jobject thisObj, jint n) {
   return (n<<2) + 3;
}
JNIEXPORT int JNICALL Java_NotFun_magic4(JNIEnv *env, jobject thisObj, jint n) {
   return (n<<2) + 1;
}
JNIEXPORT int JNICALL Java_NotFun_magic5(JNIEnv *env, jobject thisObj, jint n) {
   return (n>>2) + 3;
}
JNIEXPORT int JNICALL Java_NotFun_magic6(JNIEnv *env, jobject thisObj, jint n) {
   return (n>>3) + 3;
}
JNIEXPORT int JNICALL Java_NotFun_magic7(JNIEnv *env, jobject thisObj, jint n) {
   return (n>>1) + 3;
}
JNIEXPORT int JNICALL Java_NotFun_magic8(JNIEnv *env, jobject thisObj, jint n) {
   return (n>>4) + 7;
}
JNIEXPORT int JNICALL Java_NotFun_magic9(JNIEnv *env, jobject thisObj, jint n) {
   return (n>>1) + 1;
}




