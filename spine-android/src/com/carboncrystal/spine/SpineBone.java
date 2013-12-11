package com.carboncrystal.spine;

public class SpineBone {
	public String name;
	public SpineAttachment attachment;
	public Object userData;
	public SpineBone parent;
	public float x;
	public float y;
	public float rotation;

	public float worldX;
	public float worldY;
	public float worldRotation;

	@Override
	public String toString() {
		return name;
	}
}
