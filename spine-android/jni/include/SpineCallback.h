/*
 * SpineCallback.h
 *
 *  Created on: Oct 20, 2013
 *      Author: Jason
 */

#ifndef SPINECALLBACK_H_
#define SPINECALLBACK_H_

#include <jni.h>

class SpineCallback {

public:
	SpineCallback(JNIEnv* env, jobject jCallback);
	virtual ~SpineCallback();

	void onSkeletonCreate(JNIEnv* env, int numBones);

	void addBone(JNIEnv* env, int index, const char* name, float x, float y, float rotation, float scaleX, float scaleY);

	void onBoneStep(JNIEnv* env, int index, float x, float y, float rotation, float scaleX, float scaleY);

	void onError(JNIEnv* env, const char * format, ...);

	void destroy(JNIEnv* env);

private:
	jobject jCallback;
	jclass callbackClass;
	jmethodID stepBoneID;
	jmethodID onCreateID;
	jmethodID addBoneID;

};

#endif /* SPINECALLBACK_H_ */
