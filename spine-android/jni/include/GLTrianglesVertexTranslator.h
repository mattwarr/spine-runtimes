/*
 * GLTrianglesVertexTranslator.h
 *
 *  Created on: Nov 19, 2013
 *      Author: Jason
 */

#ifndef GLTRIANGLESVERTEXTRANSLATOR_H_
#define GLTRIANGLESVERTEXTRANSLATOR_H_

#include "VertexTranslator.h"

class GLTrianglesVertexTranslator: public VertexTranslator {
public:
	GLTrianglesVertexTranslator();
	virtual ~GLTrianglesVertexTranslator();
	int translate(float* vertices, float* output, int offset, int stride);
};

#endif /* GLTRIANGLESVERTEXTRANSLATOR_H_ */
