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

	stride -= 3; // deduct 3 from stride for x,y,z so we always start from the right position

//	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Current value at offset %d is %f\n",start, output[start]);

	// 0 (top left)
	output[offset++] = vertices[VERTEX_X1];
	output[offset++] = vertices[VERTEX_Y1];
	output[offset++] = 0; // z

//	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%f,%f,%f,%d\n",output[offset-3], output[offset-2], output[offset-1], offset-3);

	offset+=stride;

	// 1 (btm left)
	output[offset++] = vertices[VERTEX_X2];
	output[offset++] = vertices[VERTEX_Y2];
	output[offset++] = 0;

//	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%f,%f,%f,%d\n",output[offset-3], output[offset-2], output[offset-1], offset-3);

	offset+=stride;

	// 2 (top right)
	output[offset++] = vertices[VERTEX_X4];
	output[offset++] = vertices[VERTEX_Y4];
	output[offset++] = 0;

//	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%f,%f,%f,%d\n",output[offset-3], output[offset-2], output[offset-1], offset-3);

	offset+=stride;

	// 3 (btm left)
	output[offset++] = vertices[VERTEX_X2];
	output[offset++] = vertices[VERTEX_Y2];
	output[offset++] = 0;

//	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%f,%f,%f,%d\n",output[offset-3], output[offset-2], output[offset-1], offset-3);

	offset+=stride;

	// 4 (btm right)
	output[offset++] = vertices[VERTEX_X3];
	output[offset++] = vertices[VERTEX_Y3];
	output[offset++] = 0;

//	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%f,%f,%f,%d\n",output[offset-3], output[offset-2], output[offset-1], offset-3);

	offset+=stride;

	// 5 (top right)
	output[offset++] = vertices[VERTEX_X4];
	output[offset++] = vertices[VERTEX_Y4];
	output[offset++] = 0;

//	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%f,%f,%f,%d\n",output[offset-3], output[offset-2], output[offset-1], offset-3);

	offset+=stride;

//	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Returning %d\n",(offset - start));

	return offset - start;
}

