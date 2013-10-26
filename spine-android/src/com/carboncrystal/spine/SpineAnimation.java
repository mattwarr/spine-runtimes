package com.carboncrystal.spine;

public class SpineAnimation {

	public final int index;

	public SpineBone[] bones;

	private SpineAnimationListener animationListener;

	long addr;

	SpineAnimation(int index) {
		this.index = index;
	}

	final void onSkeletonCreate(int numBones) {
		bones = new SpineBone[numBones];
	}

	final void addBone(int index, String name) {
		bones[index] = new SpineBone();
		bones[index].name = name;
	}

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
		step(addr, (float)deltaTime/1000.0f); // Spine steps in seconds.
	}

	public final boolean setAnimation(int trackIndex, String name, boolean loop) {
		return setAnimation(addr, trackIndex, name, loop);
	}

	public final void destroy() {
		destroy(addr);
	}

	public void setAnimationListener(SpineAnimationListener animationListener) {
		this.animationListener = animationListener;
	}

	native void step(long addr, float deltaTime);

	native void destroy(long addr);

	native boolean setAnimation(long addr, int trackIndex, String name, boolean loop);
}
