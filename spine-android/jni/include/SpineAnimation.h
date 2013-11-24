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
#include <VertexTranslator.h>


// Correspond to android.opengl.GLES20 statics
#define GL_POINTS 			0
#define GL_LINES 			1
#define GL_LINE_LOOP 		2
#define GL_LINE_STRIP 		3
#define GL_TRIANGLES 		4
#define GL_TRIANGLE_STRIP 	5
#define GL_TRIANGLE_FAN 	6

class SpineAnimation {

public:
	SpineAnimation(JNIEnv* env, spSkeletonData* skeletonData, SpineCallback* callback);

	virtual ~SpineAnimation();

	bool setAnimation(JNIEnv* env, int trackIndex, const char* name, bool loop);

	bool addAnimation(JNIEnv* env, int trackIndex, const char* name, bool loop, float delay);

	void step(JNIEnv* env, float deltaTime);

	void destroy(JNIEnv* env);

	void init(JNIEnv* env);

	bool getBufferAddress(JNIEnv * env, jobject owner);

	void setXY(float x, float y) {
		this->x = x;
		this->y = y;
	}

private:
	spSkeleton* skeleton;
	spAnimationState* state;
	SpineCallback* callback;
	VertexTranslator* translator;
	float* vertices;
	float* buffer;
	int offset;
	int stride;
	int drawMode;
	float x;
	float y;
};

#endif /* SPINEANIMATION_H_ */
