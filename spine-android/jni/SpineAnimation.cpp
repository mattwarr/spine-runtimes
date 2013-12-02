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
#include <spine/RegionAttachment.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <math.h>

pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

static double PI2 = M_PI*2;
static int BUFFER_SIZE = 8;

SpineAnimation::SpineAnimation(JNIEnv* env, spSkeletonData* sd, SpineCallback* cb) {
	this->callback = cb;
	this->skeleton = spSkeleton_create(sd);
	this->state = spAnimationState_create(spAnimationStateData_create(skeleton->data));
	this->bounds = new spBounds();

	// This will allocate the native array
	this->callback->onSkeletonCreate(env, skeleton->slotCount);
	this->x = 0;
	this->y = 0;

	int i;

	// Update transform so the bone's world position is set
	spSkeleton_updateWorldTransform(skeleton);

	for(i = 0; i < skeleton->slotCount; i++) {
		spSlot* slot = skeleton->drawOrder[i];
		this->callback->addBone(
				env,
				i,
				slot);

		// create the buffer for this bone
		const char* boneName = slot->bone->data->name;

		this->boneVertBuffers[boneName] = new float[BUFFER_SIZE];  // 4 x 2 coords

	}

	// Loop again tp set the parents
	for(i = 0; i < skeleton->slotCount; i++) {
		spSlot* slot = skeleton->drawOrder[i];
		spBone* parent = slot->bone->parent;

		if(parent) {
			this->callback->setBoneParent(
					env,
					i,
					parent->data->name);
		}
	}

	// Set to initial pose
	spSkeleton_setToSetupPose(this->skeleton);

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

void SpineAnimation::init(JNIEnv* env) {

	this->vertices = this->callback->getVertexBuffer(env);
	this->stride = this->callback->getStride(env);
	this->drawMode = this->callback->getDrawMode(env);
	this->center = new float[2]; // x, y
	this->translator = NULL;

	switch(this->drawMode) {
		case GL_TRIANGLES:
			this->translator = new GLTrianglesVertexTranslator();
			break;
	}
}

void SpineAnimation::getAABB(JNIEnv* env) {

	bounds->minX = (float)INT_MAX;

	bounds->minY = (float)INT_MAX;

	bounds->maxX = (float)INT_MIN;

	bounds->maxY = (float)INT_MIN;

	int i, j;

	for(i = 0; i < skeleton->slotCount; i++) {

		spSlot* slot = skeleton->drawOrder[i];
		spBone* bone = slot->bone;

		float* buffer = this->boneVertBuffers [bone->data->name];

		for (j = 0; j < BUFFER_SIZE; j += 2) {

			float x = buffer[j];
			float y = buffer[j + 1];

			if (x < bounds->minX) {
				bounds->minX = x;
			}

			if (y < bounds->minY) {
				bounds->minY = y;
			}

			if (x > bounds->maxX) {
				bounds->maxX = x;
			}

			if (y > bounds->maxY) {
				bounds->maxY = y;
			}
		}
	}

	this->callback->setAABB(
			env,
			this->bounds->minX,
			this->bounds->minY,
			this->bounds->maxX,
			this->bounds->maxY);
}

void SpineAnimation::sync(JNIEnv* env) {
	int i;

	for(i = 0; i < skeleton->slotCount; i++) {
		spSlot* slot = skeleton->drawOrder[i];
		spBone* bone = slot->bone;

		float* buffer = this->boneVertBuffers [bone->data->name];

		double angle = this->calculateCenterAndAngle(buffer, this->center);

		this->callback->setBoneSRT(
				env,
				i,
				this->center[0],
				this->center[1],
				(float) angle);
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

		float* buffer = this->boneVertBuffers[ bone->data->name ];

		// Read the verts into the temp buffer
		spRegionAttachment_computeWorldVertices((spRegionAttachment*) slot->attachment, x, y, bone, buffer);

		// translate the tmp buffer into the output buffer.
		if(this->translator) {
			if(this->vertices) {
				bufferIndex = this->translator->translate( buffer, this->vertices, bufferIndex, this->stride);
			}
			else {
				callback->onError(env, "No vertex buffer found!");
				__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "No vertex buffer found!");
				break;
			}
		}
		else {
			callback->onError(env, "No vertex translator found!");
			__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "No vertex translator defined!");
			break;
		}
	}

	pthread_mutex_unlock(&mutex);
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
	delete this->center;
	delete this->bounds;

	this->boneVertBuffers.clear();

	if(this->translator) {
		delete this->translator;
	}
}

double SpineAnimation::calculateCenterAndAngle(float* vertices, float* out) {

	out[0] = (vertices[VERTEX_X1] + vertices[VERTEX_X3]) / 2.0f;
	out[1] = (vertices[VERTEX_Y1] + vertices[VERTEX_Y3]) / 2.0f;

	float diffX = vertices[VERTEX_X1] - vertices[VERTEX_X4];
	float diffY = vertices[VERTEX_Y1] - vertices[VERTEX_Y4];
	double angle = atan(diffX/diffY);

	if(diffY < 0) {
		// Adjust for 2nd & 3rd quadrants, i.e. diff y is -ve.
		angle += M_PI;
	} else if(diffX < 0) {
		// Adjust for 4th quadrant
		// i.e. diff x is -ve, diff y is +ve
		angle += PI2;
	}

	return PI2 - angle;
}

SpineAnimation::~SpineAnimation() {
	pthread_mutex_destroy ( &mutex );
}
