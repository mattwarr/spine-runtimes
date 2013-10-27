package com.carboncrystal.spine;

public class SpineBone {
	public float x;
	public float y;
	public float rotation;
	public float scaleX;
	public float scaleY;
	public String name;
	public String attachment;
	public Object userData;

	@Override
	public String toString() {
		return "SpineBone{" +
				"x=" + x +
				", y=" + y +
				", rotation=" + rotation +
				", scaleX=" + scaleX +
				", scaleY=" + scaleY +
				", name='" + name + '\'' +
				", attachment='" + attachment + '\'' +
				'}';
	}
}
