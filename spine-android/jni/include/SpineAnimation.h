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
	SpineAnimation(const char* skeletonPath, SpineCallback* callback);

	virtual ~SpineAnimation();

	bool setAnimation(int trackIndex, const char* name, bool loop);

	void step(float deltaTime);

	void initWithAtlasPath(const char* atlasPath);

	void initWithAtlasData(const char* atlasData, int length);

private:
	const char* skeletonPath;
	spSkeletonData* skeletonData;
	spSkeleton* skeleton;
	spAnimationState* state;
	SpineCallback* callback;

	void initWithAtlas(spAtlas* atlas, const char* skeletonPath);
};

#endif /* SPINEANIMATION_H_ */
