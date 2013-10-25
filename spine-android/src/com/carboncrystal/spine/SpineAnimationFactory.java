package com.carboncrystal.spine;

public class SpineAnimationFactory {

	private final long addr;
	private SpineAnimation[] animations;
	private int createIndex = 0;
	private final int size;

	public SpineAnimationFactory(String atlasPath, String skeletonPath, int size) {
		this.addr = create(atlasPath, skeletonPath);
		this.animations = new SpineAnimation[size];
		this.size = size;
	}

	public final SpineAnimation create() {
		if(createIndex < animations.length) {
			SpineAnimation animation = new SpineAnimation(createIndex);
			animation.addr = createAnimation(addr, animation);

			if(SpineContext.isNULL(animation.addr)) {
				throw new RuntimeException("Error creating animation.  Check native logs");
			}

			animations[createIndex++] = animation;
			return animation;
		}
		else {
			throw new ArrayIndexOutOfBoundsException("Exceeded max size [" +
					size +
					"] in factory");
		}
	}

	public final SpineAnimation get(int index) {
		return animations[index];
	}

	public final void destroy() {
		for (SpineAnimation animation : animations) {
			if(animation == null) {
				break;
			}
			animation.destroy();
		}
		destroy(addr);
	}

	/**
	 * Convenience method for stepping all animations
	 * @param time
	 */
	public final void step(long time) {
		for (SpineAnimation animation : animations) {
			if(animation == null) {
				break;
			}
			animation.step(time);
		}
	}

	native long create(String atlasPath, String skeletonPath);

	native long createAnimation(long addr, SpineAnimation callback);

	native void destroy(long addr);
}
