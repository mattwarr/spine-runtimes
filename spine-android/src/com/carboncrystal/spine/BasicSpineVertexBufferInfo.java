package com.carboncrystal.spine;

import android.opengl.GLES20;

import java.nio.FloatBuffer;

public class BasicSpineVertexBufferInfo extends SpineVertexBufferInfo {

	private FloatBuffer vertexBuffer;
	private int stride;
	private int drawMode = GLES20.GL_TRIANGLES;

	@Override
	public FloatBuffer getVertexBuffer() {
		return vertexBuffer;
	}

	@Override
	public int getStride() {
		return stride;
	}

	@Override
	public int getDrawMode() {
		return drawMode;
	}

	public void setVertexBuffer(FloatBuffer vertexBuffer) {
		this.vertexBuffer = vertexBuffer;
	}

	public void setStride(int stride) {
		this.stride = stride;
	}

	public void setDrawMode(int drawMode) {
		this.drawMode = drawMode;
	}
}
