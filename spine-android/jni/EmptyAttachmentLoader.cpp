/*
 * EmptyAttachmentLoader.cpp
 *
 *  Created on: Oct 26, 2013
 *      Author: Jason
 */

#include <EmptyAttachmentLoader.h>
#include <spine/extension.h>

spAttachment* _spEmptyAttachmentLoader_newAttachment (spAttachmentLoader* loader, spSkin* skin, spAttachmentType type,
		const char* name) {
	return NULL;
}

spEmptyAttachmentLoader* spEmptyAttachmentLoader_create () {
	spEmptyAttachmentLoader* self = NEW(spEmptyAttachmentLoader);
	_spAttachmentLoader_init(SUPER(self), _spAttachmentLoader_deinit, _spEmptyAttachmentLoader_newAttachment);
	return self;
}
