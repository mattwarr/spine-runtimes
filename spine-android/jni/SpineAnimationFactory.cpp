/*
 * SpineAnimationFactory.cpp
 *
 *  Created on: Oct 21, 2013
 *      Author: Jason
 */

#include <SpineAnimationFactory.h>
#include <SpineAnimation.h>
#include <EmptyAttachmentLoader.h>
#include <spine/extension.h>
#include <spine/SkeletonJson.h>
#include <android/log.h>

SpineAnimationFactory::SpineAnimationFactory(const char* atlasPath, const char* skeletonPath) {
	int length;
	this->error = false;

	spAtlas* atlas = spAtlas_readAtlasFile(atlasPath);

	spSkeletonJson* skeletonJson = spSkeletonJson_create(atlas);

	const char* json = _spUtil_readFile(skeletonPath, &length);

	this->skeletonData = spSkeletonJson_readSkeletonData(skeletonJson, json);

	if (skeletonJson->error) {
		this->error = true;
		__android_log_print(ANDROID_LOG_ERROR, "SpineAndroid",
				"Error creating spSkeletonJson: %s", skeletonJson->error);
	}

	spAtlas_dispose(atlas);
	spSkeletonJson_dispose(skeletonJson);
	FREE(json);
}

SpineAnimationFactory::~SpineAnimationFactory() {
	if (this->skeletonData != NULL) {
		spSkeletonData_dispose(this->skeletonData);
	}
}

SpineAnimation* SpineAnimationFactory::create(JNIEnv* env, SpineCallback* cb) {
	if (this->skeletonData != NULL) {
		return new SpineAnimation(env, this->skeletonData, cb);
	}
	return NULL;
}
