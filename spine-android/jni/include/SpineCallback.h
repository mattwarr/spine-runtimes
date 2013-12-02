/*
 * SpineCallback.h
 *
 *  Created on: Oct 20, 2013
 *      Author: Jason
 */

#ifndef SPINECALLBACK_H_
#define SPINECALLBACK_H_

#include <jni.h>
#include <spine/spine.h>

#include <map>

const char* const LOG_TAG = "SpineAndroid";

class SpineCallback {

public:
	SpineCallback(JNIEnv* env, jobject jCallback);
	virtual ~SpineCallback();

	void onSkeletonCreate(JNIEnv* env, int numBones);

	float* getVertexBuffer(JNIEnv* env);

	int getStride(JNIEnv* env);

	int getDrawMode(JNIEnv* env);

	void addBone(JNIEnv* env, int index, spSlot* slot);

	void setAABB(JNIEnv* env, float minX, float minY, float maxX, float maxY);

	void setBoneSRT(JNIEnv* env, int index, float worldX, float worldY, float worldRotation);

	void setBoneParent(JNIEnv* env, int index, const char * parentBoneName);

	void onError(JNIEnv* env, const char * format, ...);

	void destroy(JNIEnv* env);

private:
	jobject jCallback;
	jclass callbackClass;
	jmethodID onCreateID;
	jmethodID addBoneID;
	jmethodID setParentID;
	jmethodID setBoneSRTID;
	jmethodID setAABBID;
	jmethodID getVertexBufferID;
	jmethodID getStrideID;
	jmethodID getDrawModeID;
	std::map<const char*, int> boneMapping;
};

#endif /* SPINECALLBACK_H_ */
