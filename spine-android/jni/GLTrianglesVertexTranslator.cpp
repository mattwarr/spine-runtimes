/*
 * GLTrianglesVertexTranslator.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: Jason
 */

#include <GLTrianglesVertexTranslator.h>
#include <spine/RegionAttachment.h>
#include <android/log.h>
#include <SpineCallback.h>

GLTrianglesVertexTranslator::GLTrianglesVertexTranslator() {}

GLTrianglesVertexTranslator::~GLTrianglesVertexTranslator() {}

int GLTrianglesVertexTranslator::translate(float* vertices, float* output, int offset, int stride) {

	// GL_TRIANGLES CCW
	int start = offset;

	// 0 (top left)
	output[offset++] = vertices[VERTEX_X1];
	output[offset++] = vertices[VERTEX_Y1];
	output[offset++] = 0; // z

	offset+=stride;

	// 1 (btm left)
	output[offset++] = vertices[VERTEX_X2];
	output[offset++] = vertices[VERTEX_Y2];
	output[offset++] = 0;

	offset+=stride;

	// 2 (top right)
	output[offset++] = vertices[VERTEX_X4];
	output[offset++] = vertices[VERTEX_Y4];
	output[offset++] = 0;

	offset+=stride;

	// 3 (btm left)
	output[offset++] = vertices[VERTEX_X2];
	output[offset++] = vertices[VERTEX_Y2];
	output[offset++] = 0;

	offset+=stride;

	// 4 (btm right)
	output[offset++] = vertices[VERTEX_X3];
	output[offset++] = vertices[VERTEX_Y3];
	output[offset++] = 0;

	offset+=stride;

	// 5 (top right)
	output[offset++] = vertices[VERTEX_X4];
	output[offset++] = vertices[VERTEX_Y4];
	output[offset++] = 0;

	offset+=stride;

	return offset - start;
}

