Simple Android Spine Runtime
----------------------------

Load the lib::

	static {
		System.loadLibrary("spine");
	}

Init the context ::

	SpineContext.init(this);

Load an animation::
	
	SpineAnimation animation = new SpineAnimation("spineboy.atlas", "spineboy.json") {
		@Override
		public void onUpdate(SpineBone bone) {
		    // This will be called when each bone in the skeleton is updated.
			Log.w("SpineDemo", bone.toString());
		}
	};

	if(animation.setAnimation(1, "walk", true)) {
	    // Step with delat time
		animation.step(10);
	}

On destroy::

    animation.destroy();