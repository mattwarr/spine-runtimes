/*
 * SpineAnimation.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: Jason
 */

#include <SpineAnimation.h>
#include <SpineCallback.h>


SpineAnimation::SpineAnimation(const char* atlasPath, const char* skeletonPath, SpineCallback* cb) {

	this->callback = cb;

	spAtlas* atlas = spAtlas_readAtlasFile(atlasPath);

	spSkeletonJson* json = spSkeletonJson_create(atlas);

	skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonPath);

	if (!skeletonData) {
		this->callback->onError("Error loading skeleton data (%s) not found", json->error);
	}
	else {

		skeleton = spSkeleton_create(skeletonData);

		state = spAnimationState_create(spAnimationStateData_create(skeleton->data));

		this->callback->onSkeletonCreate(skeleton->slotCount);

		int i;

		for(i = 0; i < skeleton->slotCount; i++) {
			this->callback->addBone(i, skeleton->slots[i]->attachment->name);
		}
	}

	spSkeletonJson_dispose(json);
	spAtlas_dispose(atlas);

}

bool SpineAnimation::setAnimation(int trackIndex, const char* name, bool loop) {
	spAnimation* animation = spSkeletonData_findAnimation(skeleton->data, name);
	if (!animation) {
		callback->onError("Animation not found: %s", (char*) name);
		return false;
	}
	else {
		spAnimationState_setAnimation(state, trackIndex, animation, loop);
		return true;
	}
}

void SpineAnimation::step(float deltaTime) {

	spAnimationState_update(state, deltaTime);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	int i;
	for(i = 0; i < skeleton->boneCount; i++) {

		spBone* data = skeleton->bones[i];

		callback->onBoneStep(
				i,
				data->worldX,
				data->worldY,
				data->worldRotation,
				data->worldScaleX,
				data->worldScaleY);
	}
}

SpineAnimation::~SpineAnimation() {
	if (this->skeletonData) {
		spSkeletonData_dispose(this->skeletonData);
	}

	if (this->skeleton) {
		spSkeleton_dispose(this->skeleton);
	}

	if (this->state) {
		spAnimationState_dispose(this->state);
	}

	delete this->callback;
}
