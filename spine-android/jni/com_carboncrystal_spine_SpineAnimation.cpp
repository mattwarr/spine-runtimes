#include <jni.h>
#include <com_carboncrystal_spine_SpineAnimation.h>
#include <SpineAnimation.h>
#include <SpineCallback.h>
#include <spine_context.h>
#include <android/log.h>

JNIEXPORT void JNICALL Java_com_carboncrystal_spine_SpineAnimation_step
  (JNIEnv *env, jobject caller, jlong addr, jfloat deltaTime) {
	SpineAnimation* anim = (SpineAnimation*) addr;
	if(anim) {
		anim->step(deltaTime);
	}
}

JNIEXPORT jboolean JNICALL Java_com_carboncrystal_spine_SpineAnimation_setAnimation
  (JNIEnv *env, jobject caller, jlong addr, jint trackIndex, jstring name, jboolean loop) {
	SpineAnimation* anim = (SpineAnimation*) addr;
	if(anim) {
		const char* nName = env->GetStringUTFChars(name, 0);
		bool ok = anim->setAnimation(trackIndex, nName, loop);
		env->ReleaseStringUTFChars(name, nName);
		return ok;
	}

	return false;
}

JNIEXPORT jlong JNICALL Java_com_carboncrystal_spine_SpineAnimation_create(JNIEnv *env, jobject caller, jobject jAnimation, jstring atlasPath, jstring skeletonPath) {

	const char* aPath = env->GetStringUTFChars(atlasPath, 0);
	const char* sPath = env->GetStringUTFChars(skeletonPath, 0);

	SpineCallback* callback = new SpineCallback(jAnimation);
	SpineAnimation* animation = new SpineAnimation(aPath, sPath, callback );

	env->ReleaseStringUTFChars(atlasPath, aPath);
	env->ReleaseStringUTFChars(skeletonPath, sPath);

	return (jlong) animation;
}


JNIEXPORT void JNICALL Java_com_carboncrystal_spine_SpineAnimation_destroy(JNIEnv *e, jobject caller,  jlong addr) {
	SpineAnimation* anim = (SpineAnimation*) addr;
	if(anim) {
		delete anim;
	}
}


