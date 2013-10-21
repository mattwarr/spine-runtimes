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
	SpineCallback(jobject jCallback);
	virtual ~SpineCallback();

	void onSkeletonCreate(int numBones);

	void addBone(int index, const char* name);

	void onBoneStep(int index, float x, float y, float rotation, float scaleX, float scaleY);

	void onError(const char * format, ...);

private:
	jobject jCallback;
	jclass callbackClass;
	jmethodID stepBoneID;
	jmethodID onCreateID;
	jmethodID addBoneID;
	JNIEnv* env;

};

#endif /* SPINECALLBACK_H_ */
