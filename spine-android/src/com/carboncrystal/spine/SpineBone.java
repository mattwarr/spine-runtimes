package com.carboncrystal.spine;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class SpineBone {

	public String name;
	public SpineAttachment attachment;
	public Object userData;

	@Override
	public String toString() {
		return "SpineBone{" +
				", name='" + name + '\'' +
				", attachment=" + attachment +
				", userData=" + userData +
				'}';
	}
}
