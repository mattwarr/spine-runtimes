package com.carboncrystal.spine;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;

import java.io.IOException;

public class SpineAnimationFactory {

	private final long addr;
	private SpineAnimation[] animations;
	private int createIndex = 0;
	private final int size;

	public SpineAnimationFactory(Context context, String skeletonPath, int size) {

		// We're not going to verify on the native side, so do it here
		AssetManager am = context.getAssets();


		if(!verifyPath(am, skeletonPath)) {
			throw new RuntimeException("Cannot create factory.  Skeleton path [" +
					skeletonPath +
					"] does not exist");
		}

		this.addr = create(skeletonPath);

		if(SpineContext.isNULL(this.addr)) {
			throw new RuntimeException("Failed to create factory.  Check logs for error");
		}

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

	boolean verifyPath(AssetManager assets, String name) {
		try {
			String[] assetList = assets.list("");
			if (assetList != null && assetList.length > 0) {
				for (String item : assetList) {
					if (name.equals(item)) {
						return true;
					}
				}
			}
		}
		catch (IOException e) {
			Log.w("SpineAndroid", e);
		}
		return false;
	}

	native long create(String skeletonPath);

	native long createAnimation(long addr, SpineAnimation callback);

	native void destroy(long addr);
}
