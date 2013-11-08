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

class SpineCallback {

public:
	SpineCallback(JNIEnv* env, jobject jCallback);
	virtual ~SpineCallback();

	float* onSkeletonCreate(JNIEnv* env, int numBones);

	void addBone(JNIEnv* env, int index, spSlot* slot);

	void onError(JNIEnv* env, const char * format, ...);

	void destroy(JNIEnv* env);

private:
	jobject jCallback;
	jclass callbackClass;
//	jmethodID stepBoneID;
	jmethodID onCreateID;
	jmethodID addBoneID;
	std::map<const char*, int> boneMapping;
};

#endif /* SPINECALLBACK_H_ */
