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
import java.util.HashMap;
import java.util.Map;

public class SpineAnimationFactory {
	private static final int DEFAULT_BUFFER_SIZE = 1024 * 4;

	private final long addr;
	private SpineAnimation[] animations;
	private int createIndex = 0;
	private final int size;
	private final Map<String, String> nameLookup;

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

		// Create a local lookup for bone-to-attachment names
		nameLookup = new HashMap<String, String>();

		// Load the asset file
		try {
			JSONObject jsonObject = new JSONObject(readAsset(am, skeletonPath));
			JSONArray slots = jsonObject.getJSONArray("slots");
			for (int i = 0; i < slots.length(); i++) {
				JSONObject obj = slots.getJSONObject(i);
				nameLookup.put(obj.getString("bone"), obj.getString("attachment"));
			}
		}
		catch (Exception e) {
			throw new RuntimeException("Failed to create factory", e);
		}
	}
	public final SpineAnimation create() {
		return create((SpineAnimationInitListener) null);
	}

	public final SpineAnimation create(SpineAnimationInitListener listener) {
		if(createIndex < animations.length) {
			SpineAnimation animation = new SpineAnimation(createIndex, nameLookup);
			animation.setInitListener(listener);
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

	Map<String, String> getNameLookup() {
		return nameLookup;
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

	native long create(String skeletonPath);

	native long createAnimation(long addr, SpineAnimation callback);

	native void destroy(long addr);
}
