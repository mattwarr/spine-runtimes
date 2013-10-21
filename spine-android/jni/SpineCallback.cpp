/*
 * SpineCallback.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: Jason
 */

#include <SpineCallback.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <spine/spine.h>
#include <stdlib.h>

extern "C" {
	JNIEnv* get_jni_env();
}

const char* LOG_TAG = "SpineAndroid";

SpineCallback::SpineCallback(jobject jCallback) {
	this->env = get_jni_env();
	this->jCallback = env->NewGlobalRef(jCallback);
	callbackClass = this->env->FindClass("com/carboncrystal/spine/SpineAnimation");
	onCreateID = this->env->GetMethodID(callbackClass, "onSkeletonCreate", "(I)V");
	addBoneID = this->env->GetMethodID(callbackClass, "addBone", "(ILjava/lang/String;)V");
	stepBoneID = this->env->GetMethodID(callbackClass, "onBoneStep", "(IFFFFF)V");
}

SpineCallback::~SpineCallback() {
	env->DeleteGlobalRef(jCallback);
}

void SpineCallback::onSkeletonCreate(int numBones) {
	env->CallVoidMethod(jCallback, onCreateID, numBones);
}

void SpineCallback::addBone(int index, const char* name) {

	jstring jString = env->NewStringUTF(name);

	env->CallVoidMethod(jCallback, addBoneID, index, jString);

	env->DeleteLocalRef(jString);
}

void SpineCallback::onBoneStep(int index, float x, float y, float rotation, float scaleX, float scaleY) {
	env->CallVoidMethod(jCallback, stepBoneID, index, x, y, rotation, scaleX, scaleY);
}

void SpineCallback::onError(const char * format, ...) {
	va_list argptr;
	va_start(argptr, format);
	__android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, argptr);
	va_end(argptr);
}

