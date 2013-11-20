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

	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Translating TOP LEFT [%f,%f]", vertices[VERTEX_X1], vertices[VERTEX_Y1]);
	// 0 (top left)
	output[offset++] = vertices[VERTEX_X1];
	output[offset++] = vertices[VERTEX_Y1];

	offset+=stride;

	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Translating BTM LEFT [%f,%f]", vertices[VERTEX_X4], vertices[VERTEX_Y4]);
	// 1 (btm left)
	output[offset++] = vertices[VERTEX_X4];
	output[offset++] = vertices[VERTEX_Y4];

	offset+=stride;

	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Translating TOP RIGHT [%f,%f]", vertices[VERTEX_X2], vertices[VERTEX_Y2]);
	// 2 (top right)
	output[offset++] = vertices[VERTEX_X2];
	output[offset++] = vertices[VERTEX_Y2];

	offset+=stride;

	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Translating BTM LEFT [%f,%f]", vertices[VERTEX_X4], vertices[VERTEX_X4]);
	// 3 (btm left)
	output[offset++] = vertices[VERTEX_X4];
	output[offset++] = vertices[VERTEX_Y4];

	offset+=stride;

	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Translating BTM RIGHT [%f,%f]", vertices[VERTEX_X3], vertices[VERTEX_X3]);
	// 4 (btm right)
	output[offset++] = vertices[VERTEX_X3];
	output[offset++] = vertices[VERTEX_Y3];

	offset+=stride;

	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Translating TOP RIGHT [%f,%f]", vertices[VERTEX_X2], vertices[VERTEX_Y2]);
	// 5 (top right)
	output[offset++] = vertices[VERTEX_X2];
	output[offset++] = vertices[VERTEX_Y2];

	offset+=stride;

	return offset - start;
}

