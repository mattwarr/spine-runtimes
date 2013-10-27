/*
 * EmptyAttachmentLoader.h
 *
 *  Created on: Oct 26, 2013
 *      Author: Jason
 */

#ifndef EMPTYATTACHMENTLOADER_H_
#define EMPTYATTACHMENTLOADER_H_

#include <spine/AttachmentLoader.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	spAttachmentLoader super;
} spEmptyAttachmentLoader;

spEmptyAttachmentLoader* spEmptyAttachmentLoader_create ();

#ifdef SPINE_SHORT_NAMES
typedef spEmptyAttachmentLoader EmptyAttachmentLoader;
#define EmptyAttachmentLoader_create() spEmptyAttachmentLoader_create()
#endif

#ifdef __cplusplus
}
#endif
#endif /* EMPTYATTACHMENTLOADER_H_ */
