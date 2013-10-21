package com.carboncrystal.spine;

import android.content.Context;
import android.content.res.AssetManager;

public class SpineContext {
	public static void init(Context context) {
		init(context.getAssets());
	}
	static native void init(AssetManager manager);
}
