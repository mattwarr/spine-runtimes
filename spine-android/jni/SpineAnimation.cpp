/*
 * SpineAnimation.cpp
 *
 *  Created on: Oct 20, 2013
 *      Author: Jason
 */

#include <SpineAnimation.h>
#include <SpineCallback.h>
#include <spine/Slot.h>
#include <android/log.h>


SpineAnimation::SpineAnimation(JNIEnv* env, spSkeletonData* sd, SpineCallback* cb) {
	this->callback = cb;
	this->skeleton = spSkeleton_create(sd);
	this->state = spAnimationState_create(spAnimationStateData_create(skeleton->data));

	// This will allocate the native array
	this->vertices = this->callback->onSkeletonCreate(env, skeleton->slotCount);

	this->x = 0;
	this->y = 0;

	// TODO: Remove this
	this->skeleton->flipY = 1;

	int i;

	// Update transform so the bone's world position is set
	spSkeleton_updateWorldTransform(skeleton);

	for(i = 0; i < skeleton->slotCount; i++) {
		spSlot* slot = skeleton->drawOrder[i];
		this->callback->addBone(
				env,
				i,
				slot);
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

bool SpineAnimation::addAnimation(JNIEnv* env, int trackIndex, const char* name, bool loop, float delay) {
	spAnimation* animation = spSkeletonData_findAnimation(skeleton->data, name);
	if (!animation) {
		callback->onError(env, "Animation not found: %s", (char*) name);
		return false;
	}
	else {
		spAnimationState_addAnimation(state, trackIndex, animation, loop, delay);
		return true;
	}
}

void SpineAnimation::step(JNIEnv* env, float deltaTime) {

	spAnimationState_update(state, deltaTime);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	int i, bufferIndex = 0;

	for(i = 0; i < skeleton->slotCount; i++) {

		spSlot* slot = skeleton->drawOrder[i];

		spBone* bone = slot->bone;

		bufferIndex = i * 8; // 4x2 vert coords

		float* buffer = &vertices[bufferIndex];

		spRegionAttachment_computeWorldVertices((spRegionAttachment*) slot->attachment, x, y, bone, buffer);
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
	delete this->vertices;
}

SpineAnimation::~SpineAnimation() {}
