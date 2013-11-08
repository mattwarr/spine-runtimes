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
	onCreateID = env->GetMethodID(callbackClass, "onSkeletonCreate", "(I)Ljava/nio/FloatBuffer;");
	addBoneID = env->GetMethodID(callbackClass, "addBone", "(ILjava/lang/String;Ljava/lang/String;)V");
}

SpineCallback::~SpineCallback() {}

void SpineCallback::destroy(JNIEnv* env) {
	env->DeleteGlobalRef(jCallback);
}

float* SpineCallback::onSkeletonCreate(JNIEnv* env, int numBones) {
	jobject verts = env->CallObjectMethod(jCallback, onCreateID, numBones);
	return (float*) env->GetDirectBufferAddress(verts);
}

void SpineCallback::addBone(JNIEnv* env,
		int index,
		spSlot* slot) {

	const char* boneName = slot->attachment->name;

	this->boneMapping[boneName] = index;

	spBone* bone = slot->bone;

	jstring jSlotName = env->NewStringUTF(slot->data->name);
	jstring jBoneName = env->NewStringUTF(boneName);


	env->CallVoidMethod(jCallback, addBoneID,
			index,
			jSlotName,
			jBoneName);

	env->DeleteLocalRef(jSlotName);
	env->DeleteLocalRef(jBoneName);
}

void SpineCallback::onError(JNIEnv* env, const char * format, ...) {
	va_list argptr;
	va_start(argptr, format);
	__android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, argptr);
	va_end(argptr);
}

