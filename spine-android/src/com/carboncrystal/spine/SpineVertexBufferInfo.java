package com.carboncrystal.spine;

import java.nio.FloatBuffer;

public abstract class SpineVertexBufferInfo {

	public abstract FloatBuffer getVertexBuffer();

	public abstract int getStride();

	public abstract int getOffset();

	public abstract int getDrawMode();
}
