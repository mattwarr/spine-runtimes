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

SpineCallback::SpineCallback(JNIEnv* env, jobject jCallback) {
	this->jCallback = env->NewGlobalRef(jCallback);
	callbackClass = env->FindClass("com/carboncrystal/spine/SpineAnimation");
	onCreateID = env->GetMethodID(callbackClass, "onSkeletonCreate", "(I)V");
	addBoneID = env->GetMethodID(callbackClass, "addBone", "(ILjava/lang/String;Ljava/lang/String;)V");
	getVertexBufferID = env->GetMethodID(callbackClass, "getVertexBuffer", "()Ljava/nio/FloatBuffer;");
	getStrideID = env->GetMethodID(callbackClass, "getStride", "()I");
	getDrawModeID = env->GetMethodID(callbackClass, "getDrawMode", "()I");
	getBufferOffsetID = env->GetMethodID(callbackClass, "getBufferOffset", "()I");
}

SpineCallback::~SpineCallback() {}

void SpineCallback::destroy(JNIEnv* env) {
	env->DeleteGlobalRef(jCallback);
}

void SpineCallback::onSkeletonCreate(JNIEnv* env, int numBones) {
	env->CallVoidMethod(jCallback, onCreateID, numBones);
}

float* SpineCallback::getVertexBuffer(JNIEnv* env) {
	jobject verts = env->CallObjectMethod(jCallback, getVertexBufferID);

	if(verts) {
		return (float*) env->GetDirectBufferAddress(verts);
	}

	return NULL;
}

int SpineCallback::getBufferOffset(JNIEnv* env) {
	return (int) env->CallIntMethod(jCallback, getBufferOffsetID);
}

int SpineCallback::getStride(JNIEnv* env) {
	return (int) env->CallIntMethod(jCallback, getStrideID);
}

int SpineCallback::getDrawMode(JNIEnv* env) {
	return (int) env->CallIntMethod(jCallback, getDrawModeID);
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

