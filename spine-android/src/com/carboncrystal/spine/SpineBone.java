package com.carboncrystal.spine;

public class SpineBone {
	public float x;
	public float y;
	public float angle;
	public float scaleX;
	public float scaleY;
	public String name;

	@Override
	public String toString() {
		return "SpineBone{" +
				"x=" + x +
				", y=" + y +
				", angle=" + angle +
				", scaleX=" + scaleX +
				", scaleY=" + scaleY +
				", name='" + name + '\'' +
				'}';
	}
}
