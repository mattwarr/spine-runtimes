Simple Android Spine Runtime
----------------------------

Load the lib::

	static {
		System.loadLibrary("spine");
	}
	
	
Load an animation::
	
	SpineAnimation animation = new SpineAnimation("spineboy.atlas", "spineboy.json") {

		@Override
		public void onUpdate(SpineBone bone) {
			Log.w("SpineDemo", bone.toString());
		}
	};

	if(animation.setAnimation(1, "walk", true)) {
		animation.step(10);
		animation.destroy();
	}	