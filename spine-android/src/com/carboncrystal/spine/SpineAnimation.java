package com.carboncrystal.spine;

import android.graphics.RectF;
import android.opengl.GLES20;

import java.nio.FloatBuffer;
import java.util.Map;

public class SpineAnimation {

	public final int index;

	public SpineBone[] bones;

	private final RectF aabb = new RectF();

	public int numBones;

	private SpineAnimationListener animationListener;

	private final Map<String, SpineAttachment> slots;

	private SpineVertexBufferInfo vertexBufferInfo;

	long addr;

	boolean destroyed = false;

	SpineAnimation(int index, Map<String, SpineAttachment> slots) {
		this.index = index;
		this.slots = slots;
	}

	// Called from native
	@SuppressWarnings("unused")
	final void onSkeletonCreate(int numBones) {
		this.bones = new SpineBone[numBones];
		this.numBones = numBones;

		if(animationListener != null) {
			animationListener.onCreateSkeleton(numBones);
		}

		verifyBuffer();
	}

	public void setSpineVertexBufferInfo(SpineVertexBufferInfo vertexBufferInfo) {
		this.vertexBufferInfo = vertexBufferInfo;
		verifyBuffer();
		init(addr);
	}

	public SpineVertexBufferInfo getVertexBufferInfo() {
		return vertexBufferInfo;
	}

	/**
	 * Triggers the native side to send back bone SRT data.
	 */
	public final void syncBones() {
		sync(addr);
	}

	/**
	 * Returns the axis aligned bounding box for the animation in its current state
	 * @return
	 */
	public final RectF getAABB() {
		getAABB(addr);
		return aabb;
	}

	/**
	 * Returns the buffer into which attachment vertices will be written.
	 * CALLED FROM NATIVE CODE.
	 * @return
	 */
	@SuppressWarnings("unused")
	final FloatBuffer getVertexBuffer() {
		return (vertexBufferInfo == null) ? null : vertexBufferInfo.getVertexBuffer();
	}

	/**
	 * Returns the "stride" (distance in the buffer between successive attachments)
	 * CALLED FROM NATIVE CODE.
	 * @return
	 */
	@SuppressWarnings("unused")
	final int getStride() {
		return (vertexBufferInfo == null) ? 0 : vertexBufferInfo.getStride();
	}

	/**
	 * Returns the GL draw mode.  The QUAD vertices for each attachment will be converted into GL coordinates based on
	 * the draw mode (eg GL_TRIANGLES).
	 * CALLED FROM NATIVE CODE.
	 * @return
	 */
	@SuppressWarnings("unused")
	final int getDrawMode() {
		return (vertexBufferInfo == null) ? GLES20.GL_TRIANGLES : vertexBufferInfo.getDrawMode();
	}

	// Called from native
	@SuppressWarnings("unused")
	final void setBoneSRT(int index, float worldX, float worldY, float worldRotation) {
		SpineBone bone = bones[index];
		if(bone != null) {
			bone.worldX = worldX;
			bone.worldY = worldY;
			bone.worldRotation = worldRotation;
		}
	}

	// Called from native
	@SuppressWarnings("unused")
	final void setAABB(float minX, float minY, float maxX, float maxY) {
		aabb.set(minX, maxY, maxX, minY);
	}

	// Called from native
	@SuppressWarnings("unused")
	final void addBone(int index,
	               String slotName,
	               String boneName,
	               String attachmentName,
	               float x,
	               float y,
	               float rotation) {

		SpineBone bone = new SpineBone();

		bone.name = boneName;
		bone.x = x;
		bone.y = y;
		bone.rotation = rotation;

		// Get the attachment from the slot
		bone.attachment = slots.get(slotName);

		bones[index] = bone;

		if(animationListener != null) {
			animationListener.onCreateBone(bone);
		}
	}

	// Called from native
	@SuppressWarnings("unused")
	final void setBoneParent(int index, int parentBoneIndex) {
		SpineBone bone = bones[index];
		if(parentBoneIndex >= 0) {
			bone.parent = bones[parentBoneIndex];
		}
	}

	public final void setScreenPosition(float x, float y) {
		setXY(addr, x, y);
	}

	public final void step(long deltaTime) {
		if(!destroyed && deltaTime > 0) {
			step(addr, (float)deltaTime/1000.0f); // Spine steps in seconds.
		}
	}

	public final void draw(int offset) {
		if(!destroyed) {
			draw(addr, offset);
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

	void verifyBuffer() {
		if(vertexBufferInfo != null && numBones > 0) {

			final int FLOATS_PER_BONE = 18; // 6 x 3 coords (x,y,z)

			int capacity = vertexBufferInfo.getVertexBuffer().capacity();
			int expected = FLOATS_PER_BONE * numBones;

			if(capacity < expected) {
				throw new ArrayIndexOutOfBoundsException("Buffer capacity [" +
						capacity +
						"] is less than required [" +
						expected +
						"]");
			}
		}
	}

	native void setXY(long addr, float x, float y);

	native void draw(long addr, int offset);

	native void step(long addr, float deltaTime);

	native void destroy(long addr);

	native void init(long addr);

	native void sync(long addr);

	native void getAABB(long addr);

	native boolean setAnimation(long addr, int trackIndex, String name, boolean loop);

	native boolean addAnimation(long addr, int trackIndex, String name, boolean loop, float delay);
}
