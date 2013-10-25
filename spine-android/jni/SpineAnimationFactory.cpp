/*
 * SpineAnimationFactory.cpp
 *
 *  Created on: Oct 21, 2013
 *      Author: Jason
 */

#include <SpineAnimationFactory.h>
#include <SpineAnimation.h>
#include <spine/extension.h>
#include <android/log.h>


SpineAnimationFactory::SpineAnimationFactory(const char* atlasPath, const char* skeletonPath) {
	int length;
	spAtlas* atlas = spAtlas_readAtlasFile(atlasPath);
	spSkeletonJson* skeletonJson = spSkeletonJson_create(atlas);
	const char* json = _spUtil_readFile(skeletonPath, &length);

	__android_log_print(ANDROID_LOG_ERROR, "SpineAndroid", "Creating skeletonData, atlas is null %d", (atlas==NULL));
	__android_log_print(ANDROID_LOG_ERROR, "SpineAndroid", "Creating skeletonData, json is null %d", (json==NULL));

	this->skeletonData = spSkeletonJson_readSkeletonData(skeletonJson, json);

	__android_log_print(ANDROID_LOG_ERROR, "SpineAndroid", "Created skeletonData, skeletonData is null %d", (skeletonData==NULL));


	if(skeletonJson->error != NULL) {
		__android_log_print(ANDROID_LOG_ERROR, "SpineAndroid", "Error creating skeleton data: %s", skeletonJson->error);
	}

	spSkeletonJson_dispose(skeletonJson);
	FREE(json);
	spAtlas_dispose(atlas);
}

SpineAnimationFactory::~SpineAnimationFactory() {
	if(this->skeletonData != NULL) {
		spSkeletonData_dispose(this->skeletonData);
	}
}

SpineAnimation* SpineAnimationFactory::create(JNIEnv* env, SpineCallback* cb) {
	if(this->skeletonData != NULL) {
		return new SpineAnimation(env, this->skeletonData, cb);
	}
	return NULL;
}
