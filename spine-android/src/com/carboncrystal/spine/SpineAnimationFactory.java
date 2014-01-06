package com.carboncrystal.spine;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;
import org.json.JSONArray;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class SpineAnimationFactory {
	private static final int DEFAULT_BUFFER_SIZE = 1024 * 4;

	private final long addr;
	private ArrayList<SpineAnimation> animations;
	private int createIndex = 0;
	private final Map<String, SpineAttachment> slots;

	public SpineAnimationFactory(Context context, String atlasPath, String skeletonPath) {

		// We're not going to verify on the native side, so do it here
		AssetManager am = context.getAssets();

		if(!verifyPath(am, atlasPath)) {
			throw new RuntimeException("Cannot create factory.  Atlas path [" +
					atlasPath +
					"] does not exist");
		}

		if(!verifyPath(am, skeletonPath)) {
			throw new RuntimeException("Cannot create factory.  Skeleton path [" +
					skeletonPath +
					"] does not exist");
		}

		this.addr = create(atlasPath, skeletonPath);

		if(SpineContext.isNULL(this.addr)) {
			throw new RuntimeException("Failed to create factory.  Check logs for error");
		}

		this.animations = new ArrayList<SpineAnimation>();

		// Create a local lookup for bone-to-attachment names
		slots = new HashMap<String, SpineAttachment>();

		// Load the asset file to get the names of the bones
		try {
			JSONObject jsonObject = new JSONObject(readAsset(am, skeletonPath));

			JSONArray slots = jsonObject.getJSONArray("slots");

			for (int i = 0; i < slots.length(); i++) {
				JSONObject obj = slots.getJSONObject(i);

				if(obj.has("attachment") && !obj.isNull("attachment")) {
					String name = obj.getString("attachment");
					String slot = obj.getString("name");
					SpineAttachment attachment = new SpineAttachment(name);
					this.slots.put(slot, attachment);
				}
			}
		}
		catch (Exception e) {
			throw new RuntimeException("Failed to create factory", e);
		}
	}
	public final SpineAnimation create() {
		return create(null);
	}

	public final SpineAnimation create(SpineAnimationListener listener) {
		SpineAnimation animation = new SpineAnimation(createIndex++, slots);
		animation.setAnimationListener(listener);
		animation.addr = createAnimation(addr, animation);
		if(SpineContext.isNULL(animation.addr)) {
			throw new RuntimeException("Error creating animation.  Check native logs");
		}
		animations.add(animation);
		return animation;
	}

	public final SpineAnimation get(int index) {
		return animations.get(index);
	}

	public final void destroy() {
		for (SpineAnimation animation : animations) {
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

	String readAsset(AssetManager am, String name) throws IOException {
		return readStream(am.open(name));
	}

	String readStream(InputStream input) throws IOException {
		InputStreamReader reader = new InputStreamReader(input);
		StringWriter writer = new StringWriter();
		char[] buffer = new char[DEFAULT_BUFFER_SIZE];
		int n;
		while (-1 != (n = reader.read(buffer))) {
			writer.write(buffer, 0, n);
		}
		return writer.toString();
	}

	native long create(String atlasPath, String skeletonPath);

	native long createAnimation(long addr, SpineAnimation callback);

	native void destroy(long addr);
}
