package com.carboncrystal.spine;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class SpineBone {

	public final SpineSkin skin;
	public String name;
	public SpineAttachment attachment;
	public Object userData;

	public SpineBone() {
		skin = new SpineSkin();
	}

	@Override
	public String toString() {
		return "SpineBone{" +
				", skin=" + skin +
				", name='" + name + '\'' +
				", attachment=" + attachment +
				", userData=" + userData +
				'}';
	}
}
