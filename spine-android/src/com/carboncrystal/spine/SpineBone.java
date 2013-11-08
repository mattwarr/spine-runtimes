package com.carboncrystal.spine;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class SpineBone {
//	public final SRT worldSRT;
//	public final SRT localSRT;
	public final SpineSkin skin;
	public String name;
	public SpineAttachment attachment;
	public Object userData;



	public SpineBone() {
//		worldSRT = new SRT();
//		localSRT = new SRT();
		skin = new SpineSkin();
	}

	@Override
	public String toString() {
		return "SpineBone{" +
//				"worldSRT=" + worldSRT +
//				", localSRT=" + localSRT +
				", skin=" + skin +
				", name='" + name + '\'' +
				", attachment=" + attachment +
				", userData=" + userData +
				'}';
	}
}
