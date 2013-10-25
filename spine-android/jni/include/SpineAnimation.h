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

	void step(JNIEnv* env, float deltaTime);

	void destroy(JNIEnv* env);

private:
	spSkeleton* skeleton;
	spAnimationState* state;
	SpineCallback* callback;
};

#endif /* SPINEANIMATION_H_ */
