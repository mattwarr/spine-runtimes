package com.carboncrystal.spine;

import java.nio.FloatBuffer;
import java.util.Map;

public class SpineAnimation {

	public final int index;

	public SpineBone[] bones;

	public int numBones;

	private SpineAnimationListener animationListener;

	private final Map<String, SpineAttachment> slots;

	private FloatBuffer vertices;
	private int stride;
	private int offset;
	private int drawMode;

	private final SpineVertexBufferInfo vertexBufferInfo;

	long addr;

	boolean destroyed = false;

	SpineAnimation(int index, Map<String, SpineAttachment> slots, SpineVertexBufferInfo vertexBufferInfo) {
		this.index = index;
		this.slots = slots;
		this.vertexBufferInfo = vertexBufferInfo;
	}

	// Called from native
	@SuppressWarnings("unused")
	final void onSkeletonCreate(int numBones) {
		this.bones = new SpineBone[numBones];
		this.numBones = numBones;

		if(animationListener != null) {
			animationListener.onCreateSkeleton(numBones);
		}

		this.vertices = vertexBufferInfo.getVertexBuffer();
		this.stride = vertexBufferInfo.getStride();
		this.offset = vertexBufferInfo.getOffset();
		this.drawMode = vertexBufferInfo.getDrawMode();
	}

	/**
	 * Returns the buffer into which attachment vertices will be written.
	 * CALLED FROM NATIVE CODE.
	 * @return
	 */
	@SuppressWarnings("unused")
	public final FloatBuffer getVertexBuffer() {
		return vertices;
	}

	/**
	 * Returns the "stride" (distance in the buffer between successive attachments)
	 * CALLED FROM NATIVE CODE.
	 * @return
	 */
	@SuppressWarnings("unused")
	public final int getStride() {
		return stride;
	}

	/**
	 * Returns the GL draw mode.  The QUAD vertices for each attachment will be converted into GL coordinates based on
	 * the draw mode (eg GL_TRIANGLES).
	 * CALLED FROM NATIVE CODE.
	 * @return
	 */
	@SuppressWarnings("unused")
	public final int getDrawMode() {
		return drawMode;
	}

	/**
	 * Returns the offset into the buffer that this animation will start writing floats.
	 * CALLED FROM NATIVE CODE.
	 * @return
	 */
	@SuppressWarnings("unused")
	public final int getBufferOffset() {
		return offset;
	}

	// Called from native
	@SuppressWarnings("unused")
	final void addBone(int index,
	               String slotName,
	               String boneName) {

		SpineBone bone = new SpineBone();

		bone.name = boneName;

		// Get the attachment from the slot
		bone.attachment = slots.get(slotName);

		bones[index] = bone;

		if(animationListener != null) {
			animationListener.onCreateBone(bone);
		}
	}

	public final void setXY(float x, float y) {
		setXY(addr, x, y);
	}

	public final void step(long deltaTime) {
		if(!destroyed && deltaTime > 0) {
			step(addr, (float)deltaTime/1000.0f); // Spine steps in seconds.
		}
	}

	public final boolean setAnimation(int trackIndex, String name, boolean loop) {
		return !destroyed && setAnimation(addr, trackIndex, name, loop);
	}

	public final boolean addAnimation(int trackIndex, String name, boolean loop, float delay) {
		return !destroyed && addAnimation(addr, trackIndex, name, loop, delay);
	}

	public final void destroy() {
		if(!destroyed) {
			destroyed = true;
			destroy(addr);
		}
	}

	void setAnimationListener(SpineAnimationListener animationListener) {
		this.animationListener = animationListener;
	}

	native void setXY(long addr, float x, float y);

	native void step(long addr, float deltaTime);

	native void destroy(long addr);

	native boolean setAnimation(long addr, int trackIndex, String name, boolean loop);

	native boolean addAnimation(long addr, int trackIndex, String name, boolean loop, float delay);
}
