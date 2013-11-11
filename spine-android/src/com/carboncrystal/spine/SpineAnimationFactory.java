package com.carboncrystal.spine;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StringWriter;
import java.util.*;

public class SpineAnimationFactory {
	private static final int DEFAULT_BUFFER_SIZE = 1024 * 4;

	private final long addr;
	private SpineAnimation[] animations;
	private int createIndex = 0;
	private final int size;
	private final Map<String, SpineAttachment> slots;

	public SpineAnimationFactory(Context context, String atlasPath, String skeletonPath, int size) {
		this(context, atlasPath, skeletonPath, null, size);
	}

	public SpineAnimationFactory(Context context, String atlasPath, String skeletonPath, String skin, int size) {

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

		this.animations = new SpineAnimation[size];
		this.size = size;

		// Create a local lookup for bone-to-attachment names
		slots = new HashMap<String, SpineAttachment>();

		// Load the asset file to get the names of the bones
		try {
			JSONObject jsonObject = new JSONObject(readAsset(am, skeletonPath));

			JSONArray slots = jsonObject.getJSONArray("slots");

			for (int i = 0; i < slots.length(); i++) {
				JSONObject obj = slots.getJSONObject(i);
				String name = obj.getString("attachment");
				String slot = obj.getString("name");
				SpineAttachment attachment = new SpineAttachment(name);
				this.slots.put(slot, attachment);
			}
//
//			JSONObject skins = jsonObject.getJSONObject("skins");
//
//			if(skin == null) {
//				skin = "default";
//			}
//
//			JSONObject skinObj = skins.getJSONObject(skin);
//
//			Iterator<?> slotNames = skinObj.keys();
//
//			while(slotNames.hasNext()) {
//				String slot = slotNames.next().toString();
//
//				JSONObject slotData = skinObj.getJSONObject(slot);
//
//				SpineAttachment attachment = this.slots.get(slot);
//
//				JSONObject skinData = slotData.getJSONObject(attachment.name);
//
//				attachment.skin = new SpineSkin();
//				attachment.skin.x = getJSONFloat(skinData, "x");
//				attachment.skin.y = getJSONFloat(skinData, "y");
//				attachment.skin.rotation = getJSONFloat(skinData, "rotation");
//				attachment.skin.width = getJSONFloat(skinData, "width");
//				attachment.skin.height = getJSONFloat(skinData, "height");
//			}
		}
		catch (Exception e) {
			throw new RuntimeException("Failed to create factory", e);
		}
	}
	public final SpineAnimation create() {
		return create(null);
	}

//	float getJSONFloat(JSONObject object, String key) throws JSONException {
//		if(object.has(key) && !object.isNull(key)) {
//			return (float) object.getDouble(key);
//		}
//		return 0.0f;
//	}

	public final SpineAnimation create(SpineAnimationListener listener) {
		if(createIndex < animations.length) {
			SpineAnimation animation = new SpineAnimation(createIndex, slots);
			animation.setAnimationListener(listener);
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
