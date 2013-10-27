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

const char* LOG_TAG = "SpineAndroid";

SpineCallback::SpineCallback(JNIEnv* env, jobject jCallback) {
	this->jCallback = env->NewGlobalRef(jCallback);
	callbackClass = env->FindClass("com/carboncrystal/spine/SpineAnimation");
	onCreateID = env->GetMethodID(callbackClass, "onSkeletonCreate", "(I)V");
	addBoneID = env->GetMethodID(callbackClass, "addBone", "(ILjava/lang/String;FFFFF)V");
	stepBoneID = env->GetMethodID(callbackClass, "onBoneStep", "(IFFFFF)V");
}

SpineCallback::~SpineCallback() {}

void SpineCallback::destroy(JNIEnv* env) {
	env->DeleteGlobalRef(jCallback);
}

void SpineCallback::onSkeletonCreate(JNIEnv* env, int numBones) {
	env->CallVoidMethod(jCallback, onCreateID, numBones);
}

void SpineCallback::addBone(JNIEnv* env, int index, const char* name, float x, float y, float rotation, float scaleX, float scaleY) {
	jstring jString = env->NewStringUTF(name);
	env->CallVoidMethod(jCallback, addBoneID, index, jString, x, y, rotation, scaleX, scaleY);
	env->DeleteLocalRef(jString);
}

void SpineCallback::onBoneStep(JNIEnv* env, int index, float x, float y, float rotation, float scaleX, float scaleY) {
	env->CallVoidMethod(jCallback, stepBoneID, index, x, y, rotation, scaleX, scaleY);
}

void SpineCallback::onError(JNIEnv* env, const char * format, ...) {
	va_list argptr;
	va_start(argptr, format);
	__android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, argptr);
	va_end(argptr);
}

