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
#include <pthread.h>
#include <errno.h>
#include <string.h>

pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

SpineAnimation::SpineAnimation(JNIEnv* env, spSkeletonData* sd, SpineCallback* cb) {
	this->callback = cb;
	this->skeleton = spSkeleton_create(sd);
	this->state = spAnimationState_create(spAnimationStateData_create(skeleton->data));

	// This will allocate the native array
	this->callback->onSkeletonCreate(env, skeleton->slotCount);
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

	// Initialize the mutex to lock between draw and step
	int mutexInitState = pthread_mutex_init (&mutex , NULL );

	if(mutexInitState != 0) {
		callback->onError(env, "Error initializing mutex: %s", strerror(errno));
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

	pthread_mutex_lock(&mutex);

	spAnimationState_update(state, deltaTime);
	spAnimationState_apply(state, skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	pthread_mutex_unlock(&mutex);
}

void SpineAnimation::draw(JNIEnv* env, int offset) {

	int i, bufferIndex = offset;

	pthread_mutex_lock(&mutex);

	for(i = 0; i < skeleton->slotCount; i++) {

		spSlot* slot = skeleton->drawOrder[i];

		spBone* bone = slot->bone;

		// Read the verts into the temp buffer
		spRegionAttachment_computeWorldVertices((spRegionAttachment*) slot->attachment, x, y, bone, this->buffer);

		// translate the tmp buffer into the output buffer.
		if(this->translator) {
			if(this->vertices) {
				bufferIndex += this->translator->translate(this->buffer, this->vertices, bufferIndex, this->stride);
			}
			else {
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "No vertex buffer found!");
				break;
			}
		}
		else {
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "No vertex translator defined!");
			break;
		}
	}

	pthread_mutex_unlock(&mutex);
}

void SpineAnimation::init(JNIEnv* env) {
	this->vertices = this->callback->getVertexBuffer(env);
	this->stride = this->callback->getStride(env);
	this->drawMode = this->callback->getDrawMode(env);
	this->buffer = new float[8]; // 4x2 vert coords
	this->translator = NULL;

	switch(this->drawMode) {
		case GL_TRIANGLES:
			this->translator = new GLTrianglesVertexTranslator();
			break;
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

SpineAnimation::~SpineAnimation() {
	pthread_mutex_destroy ( &mutex );
}
