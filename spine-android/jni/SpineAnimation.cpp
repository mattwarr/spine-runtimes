/*
 * SpineAnimation.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: Jason
 */

#include <SpineAnimation.h>
#include <SpineCallback.h>

SpineAnimation::SpineAnimation(JNIEnv* env, spSkeletonData* sd, SpineCallback* cb) {
	this->callback = cb;
	this->skeleton = spSkeleton_create(sd);
	this->state = spAnimationState_create(spAnimationStateData_create(skeleton->data));
	this->callback->onSkeletonCreate(env, skeleton->slotCount);

	int i;

	for(i = 0; i < skeleton->slotCount; i++) {
		this->callback->addBone(env, i, skeleton->slots[i]->attachment->name);
	}
}

bool SpineAnimation::setAnimation(JNIEnv* env, int trackIndex, const char* name, bool loop) {
	spAnimation* animation = spSkeletonData_findAnimation(skeleton->data, name);
	if (!animation) {
		callback->onError(env, "Animation not found: %s", (char*) name);
		return false;
	}
	else {
		spAnimationState_setAnimation(state, trackIndex, animation, loop);
		return true;
	}
}

void SpineAnimation::step(JNIEnv* env, float deltaTime) {

	spAnimationState_update(state, deltaTime);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	int i;
	for(i = 0; i < skeleton->slotCount; i++) {

		spBone* data = skeleton->slots[i]->bone;

		callback->onBoneStep(
				env,
				i,
				data->worldX,
				data->worldY,
				data->worldRotation,
				data->worldScaleX,
				data->worldScaleY);
	}
}

void SpineAnimation::destroy(JNIEnv* env) {
	if (this->skeleton) {
		spSkeleton_dispose(this->skeleton);
	}

	if (this->state) {
		spAnimationState_dispose(this->state);
	}

	this->callback->destroy(env);

	delete this->callback;
}

SpineAnimation::~SpineAnimation() {}
