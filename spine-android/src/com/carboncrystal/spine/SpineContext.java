package com.carboncrystal.spine;

import android.content.Context;
import android.content.res.AssetManager;

public class SpineContext {
	static long NULL;
	public static void init(Context context) {
		NULL = init(context.getAssets());
	}
	static native long init(AssetManager manager);
	static boolean isNULL(long addr) {
		return addr == NULL;
	}
}
