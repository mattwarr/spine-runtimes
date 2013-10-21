package com.carboncrystal.spine;

public abstract class SpineAnimation {

	private SpineBone[] bones;

	private final String atlasPath;
	private final String skeletonPath;

	long addr;

	protected SpineAnimation(String atlasPath, String skeletonPath) {
		this.atlasPath = atlasPath;
		this.skeletonPath = skeletonPath;
		this.addr = create(this, atlasPath, skeletonPath);
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
		onUpdate(bone);
	}

	public abstract void onUpdate(SpineBone bone);

	public final void step(long deltaTime) {
		step(addr, deltaTime);
	}

	public final boolean setAnimation(int trackIndex, String name, boolean loop) {
		return setAnimation(addr, trackIndex, name, loop);
	}

	public final void destroy() {
		destroy(addr);
	}

	native void step(long addr, float deltaTime);

	native void destroy(long addr);

	native boolean setAnimation(long addr, int trackIndex, String name, boolean loop);

	native long create(SpineAnimation animation, String atlasPath, String skeletonPath);
}
