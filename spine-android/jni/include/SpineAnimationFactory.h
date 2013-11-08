/*
 * SpineAnimationFactory.h
 *
 *  Created on: Oct 21, 2013
 *      Author: Jason
 */

#ifndef SPINEANIMATIONFACTORY_H_
#define SPINEANIMATIONFACTORY_H_

#include <spine/spine.h>
#include <SpineAnimation.h>
#include <SpineCallback.h>
#include <spine/AttachmentLoader.h>

class SpineAnimationFactory {
public:
	SpineAnimationFactory(const char* atlasPath, const char* skeletonPath);
	virtual ~SpineAnimationFactory();

	SpineAnimation* create(JNIEnv* env, SpineCallback* cb);

	bool inError() {
		return this->error;
	}

private:
	spSkeletonData* skeletonData;
//	spAttachmentLoader* attachmentLoader;
	bool error;
};

#endif /* SPINEANIMATIONFACTORY_H_ */
