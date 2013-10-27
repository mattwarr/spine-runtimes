package com.carboncrystal.spine;

public class SpineAnimation {

	public final int index;

	public SpineBone[] bones;

	private SpineAnimationListener animationListener;

	long addr;
	boolean destroyed = false;

	SpineAnimation(int index) {
		this.index = index;
	}

	// Called from native
	@SuppressWarnings("unused")
	final void onSkeletonCreate(int numBones) {
		bones = new SpineBone[numBones];
	}

	// Called from native
	@SuppressWarnings("unused")
	final void addBone(int index, String name) {
		bones[index] = new SpineBone();
		bones[index].name = name;
	}

	// Called from native
	@SuppressWarnings("unused")
	final void onBoneStep(int index, float x, float y, float rotation, float scaleX, float scaleY) {
		SpineBone bone = bones[index];
		bone.x = x;
		bone.y = y;
		bone.angle = rotation;
		bone.scaleX = scaleX;
		bone.scaleY = scaleY;

		if(animationListener != null) {
			animationListener.onUpdate(bone);
		}
	}

	public final void step(long deltaTime) {
		if(!destroyed) {
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

	public void setAnimationListener(SpineAnimationListener animationListener) {
		this.animationListener = animationListener;
	}

	native void step(long addr, float deltaTime);

	native void destroy(long addr);

	native boolean setAnimation(long addr, int trackIndex, String name, boolean loop);

	native boolean addAnimation(long addr, int trackIndex, String name, boolean loop, float delay);
}
