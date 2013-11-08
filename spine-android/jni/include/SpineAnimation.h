/*
 * SpineAnimation.h
 *
 *  Created on: Oct 20, 2013
 *      Author: Jason
 */

#ifndef SPINEANIMATION_H_
#define SPINEANIMATION_H_

#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <spine/spine.h>
#include <SpineCallback.h>

class SpineAnimation {

public:
	SpineAnimation(JNIEnv* env, spSkeletonData* skeletonData, SpineCallback* callback);

	virtual ~SpineAnimation();

	bool setAnimation(JNIEnv* env, int trackIndex, const char* name, bool loop);

	bool addAnimation(JNIEnv* env, int trackIndex, const char* name, bool loop, float delay);

	void step(JNIEnv* env, float deltaTime);

	void destroy(JNIEnv* env);

	bool getBufferAddress(JNIEnv * env, jobject owner);

	void setXY(float x, float y) {
		this->x = x;
		this->y = y;
	}

private:
	spSkeleton* skeleton;
	spAnimationState* state;
	SpineCallback* callback;
	float* vertices;
	float x;
	float y;
};

#endif /* SPINEANIMATION_H_ */
