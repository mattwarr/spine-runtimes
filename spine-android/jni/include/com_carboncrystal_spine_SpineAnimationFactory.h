/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_carboncrystal_spine_SpineAnimationFactory */

#ifndef _Included_com_carboncrystal_spine_SpineAnimationFactory
#define _Included_com_carboncrystal_spine_SpineAnimationFactory
#ifdef __cplusplus
extern "C" {
#endif
#undef com_carboncrystal_spine_SpineAnimationFactory_DEFAULT_BUFFER_SIZE
#define com_carboncrystal_spine_SpineAnimationFactory_DEFAULT_BUFFER_SIZE 4096L
/*
 * Class:     com_carboncrystal_spine_SpineAnimationFactory
 * Method:    create
 * Signature: (Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_carboncrystal_spine_SpineAnimationFactory_create
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     com_carboncrystal_spine_SpineAnimationFactory
 * Method:    createAnimation
 * Signature: (JLcom/carboncrystal/spine/SpineAnimation;)J
 */
JNIEXPORT jlong JNICALL Java_com_carboncrystal_spine_SpineAnimationFactory_createAnimation
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     com_carboncrystal_spine_SpineAnimationFactory
 * Method:    destroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_carboncrystal_spine_SpineAnimationFactory_destroy
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
