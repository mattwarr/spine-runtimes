package com.carboncrystal.spine;

import java.nio.FloatBuffer;

public class WrapperSpineVertexBufferInfo extends SpineVertexBufferInfo {

	private final FloatBuffer vertexBuffer;

	private int stride;
	private int drawMode;

	public WrapperSpineVertexBufferInfo(FloatBuffer vertexBuffer) {
		this.vertexBuffer = vertexBuffer;
	}

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

	public void setStride(int stride) {
		this.stride = stride;
	}

	public void setDrawMode(int drawMode) {
		this.drawMode = drawMode;
	}
}
