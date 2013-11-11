package com.carboncrystal.spine;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Map;

public class SpineAnimation {

	public final int index;

	public SpineBone[] bones;

	private SpineAnimationListener animationListener;

	private final Map<String, SpineAttachment> slots;

	static final int BYTES_PER_FLOAT = 4;
	static final int FLOATS_PER_BONE = 8;

	public FloatBuffer vertices;

	long addr;
	boolean destroyed = false;

	SpineAnimation(int index, Map<String, SpineAttachment> slots) {
		this.index = index;
		this.slots = slots;
	}

	// Called from native
	@SuppressWarnings("unused")
	final FloatBuffer onSkeletonCreate(int numBones) {
		bones = new SpineBone[numBones];

		// Allocate the native buffer
		vertices = ByteBuffer.allocateDirect(BYTES_PER_FLOAT * FLOATS_PER_BONE * numBones).order(ByteOrder.nativeOrder()).asFloatBuffer();

		return vertices;
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

	// Called from native
	@SuppressWarnings("unused")
	final FloatBuffer getVertices() {
		return vertices;
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
