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
#include <GLTrianglesVertexTranslator.h>

SpineAnimation::SpineAnimation(JNIEnv* env, spSkeletonData* sd, SpineCallback* cb) {
	this->callback = cb;
	this->skeleton = spSkeleton_create(sd);
	this->state = spAnimationState_create(spAnimationStateData_create(skeleton->data));

	// This will allocate the native array
	this->callback->onSkeletonCreate(env, skeleton->slotCount);

	this->vertices = this->callback->getVertexBuffer(env);
	this->stride = this->callback->getStride(env);
	this->offset = this->callback->getBufferOffset(env);
	this->drawMode = this->callback->getDrawMode(env);
	this->buffer = new float[8]; // 4x2 vert coords

	switch(this->drawMode) {

	this->translator = NULL;

	case GL_TRIANGLES:
		this->translator = new GLTrianglesVertexTranslator();
		break;
	}

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

	int i, bufferIndex = this->offset;

	for(i = 0; i < skeleton->slotCount; i++) {

		spSlot* slot = skeleton->drawOrder[i];

		spBone* bone = slot->bone;

		// Read the verts into the temp buffer
		spRegionAttachment_computeWorldVertices((spRegionAttachment*) slot->attachment, x, y, bone, this->buffer);

		// translate the tmp buffer into the output buffer.
		if(this->translator) {
			bufferIndex += this->translator->translate(this->buffer, this->vertices, bufferIndex, this->stride);
		}
		else {
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "No vertex translator defined!");
			break;
		}
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
	delete this->buffer;

	if(this->translator) {
		delete this->translator;
	}
}

SpineAnimation::~SpineAnimation() {}
