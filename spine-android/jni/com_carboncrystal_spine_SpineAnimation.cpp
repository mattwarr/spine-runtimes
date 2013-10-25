#include <jni.h>
#include <com_carboncrystal_spine_SpineAnimation.h>
#include <SpineAnimation.h>

JNIEXPORT void JNICALL Java_com_carboncrystal_spine_SpineAnimation_step
  (JNIEnv *env, jobject caller, jlong addr, jfloat deltaTime) {
	SpineAnimation* anim = (SpineAnimation*) addr;
	if(anim) {
		anim->step(env, deltaTime);
	}
}

JNIEXPORT jboolean JNICALL Java_com_carboncrystal_spine_SpineAnimation_setAnimation
  (JNIEnv *env, jobject caller, jlong addr, jint trackIndex, jstring name, jboolean loop) {
	SpineAnimation* anim = (SpineAnimation*) addr;
	if(anim) {
		const char* nName = env->GetStringUTFChars(name, 0);
		bool ok = anim->setAnimation(env, trackIndex, nName, loop);
		env->ReleaseStringUTFChars(name, nName);
		return ok;
	}
	return false;
}


JNIEXPORT void JNICALL Java_com_carboncrystal_spine_SpineAnimation_destroy(JNIEnv *e, jobject caller,  jlong addr) {
	SpineAnimation* anim = (SpineAnimation*) addr;
	if(anim) {
		anim->destroy(e);
		delete anim;
	}
}
