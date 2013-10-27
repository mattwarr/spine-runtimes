#include <jni.h>
#include <com_carboncrystal_spine_SpineAnimationFactory.h>
#include <SpineAnimationFactory.h>

/*
 * Class:     com_carboncrystal_spine_SpineAnimationFactory
 * Method:    create
 * Signature: (Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_carboncrystal_spine_SpineAnimationFactory_create
  (JNIEnv *env, jobject caller, jstring skeletonPath) {
	const char* sPath = env->GetStringUTFChars(skeletonPath, 0);

	SpineAnimationFactory* factory = new SpineAnimationFactory(sPath);

	env->ReleaseStringUTFChars(skeletonPath, sPath);

	if(factory->inError()) {
		return NULL;
	}

	return (jlong) factory;
}

/*
 * Class:     com_carboncrystal_spine_SpineAnimationFactory
 * Method:    createAnimation
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_carboncrystal_spine_SpineAnimationFactory_createAnimation
  (JNIEnv *env, jobject caller, jlong addr, jobject jCallback) {
	SpineAnimationFactory* factory = (SpineAnimationFactory*) addr;
	SpineAnimation* anim = factory->create(env, new SpineCallback(env, jCallback));
	return (jlong) anim;
}

/*
 * Class:     com_carboncrystal_spine_SpineAnimationFactory
 * Method:    destroy
 * Signature: (J)J
 */
JNIEXPORT void JNICALL Java_com_carboncrystal_spine_SpineAnimationFactory_destroy
  (JNIEnv *env, jobject caller, jlong addr) {
	SpineAnimationFactory* factory = (SpineAnimationFactory*) addr;
	if(factory) {
		delete factory;
	}
}
