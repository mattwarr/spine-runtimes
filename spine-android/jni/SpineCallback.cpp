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
	addBoneID = env->GetMethodID(callbackClass, "addBone", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;FF)V");
	setParentID = env->GetMethodID(callbackClass, "setBoneParent", "(II)V");
	setBoneSRTID = env->GetMethodID(callbackClass, "setBoneSRT", "(IFFF)V");
	getVertexBufferID = env->GetMethodID(callbackClass, "getVertexBuffer", "()Ljava/nio/FloatBuffer;");
	getStrideID = env->GetMethodID(callbackClass, "getStride", "()I");
	getDrawModeID = env->GetMethodID(callbackClass, "getDrawMode", "()I");
	setAABBID = env->GetMethodID(callbackClass, "setAABB", "(FFFF)V");
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

int SpineCallback::getStride(JNIEnv* env) {
	return (int) env->CallIntMethod(jCallback, getStrideID);
}

int SpineCallback::getDrawMode(JNIEnv* env) {
	return (int) env->CallIntMethod(jCallback, getDrawModeID);
}

void SpineCallback::addBone(JNIEnv* env,
		int index,
		spSlot* slot) {

	spBone* bone = slot->bone;

	const char* boneName = bone->data->name;
	const char* attachmentName = slot->attachment->name;

	this->boneMapping[boneName] = index;

	float x = 0, y = 0;

	jstring jSlotName = env->NewStringUTF(slot->data->name);
	jstring jBoneName = env->NewStringUTF(boneName);
	jstring jAttachmentName = env->NewStringUTF(attachmentName);

	env->CallVoidMethod(jCallback, addBoneID,
			index,
			jSlotName,
			jBoneName,
			jAttachmentName,
			bone->x,
			bone->y);

	env->DeleteLocalRef(jSlotName);
	env->DeleteLocalRef(jBoneName);
	env->DeleteLocalRef(jAttachmentName);
}

void SpineCallback::setBoneSRT(JNIEnv* env, int index, float worldX, float worldY, float worldRotation) {
	env->CallVoidMethod(jCallback, setBoneSRTID,
			index,
			worldX,
			worldY,
			worldRotation);
}


void SpineCallback::setAABB(JNIEnv* env, float minX, float minY, float maxX, float maxY) {
	env->CallVoidMethod(jCallback, setAABBID,
			minX,
			minY,
			maxX,
			maxY);
}

void SpineCallback::setBoneParent(JNIEnv* env, int index, const char * parentBoneName) {

	if ( this->boneMapping.find(parentBoneName) != this->boneMapping.end() ) {
		int parentId = this->boneMapping[parentBoneName];

		env->CallVoidMethod(jCallback, setParentID,
				index,
				parentId);
	}
}

void SpineCallback::onError(JNIEnv* env, const char * format, ...) {
	va_list argptr;
	va_start(argptr, format);
	__android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, format, argptr);
	va_end(argptr);
}

