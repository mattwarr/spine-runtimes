Simple Android Spine Runtime
----------------------------

Build the library::

    ndk-build

Load the lib::

	static {
		System.loadLibrary("spine");
	}

Init the context ::

	SpineContext.init(this);

Create a factory (number of animations must be specified) ::

	SpineAnimationFactory factory = new SpineAnimationFactory(this, "spineboy.json", 1);

Create an animation::

	SpineAnimation animation = factory.create();

Set the animation and track::

    animation.setAnimation(0, "walk", true);

Optionally add a listener::

    animation.setAnimationListener(new SpineAnimationListener() {
        @Override
        public void onUpdate(SpineBone bone) {
            // Called for each bone during step
        }
    });

Step the animation::

	animation.step(delta); // delta is in milliseconds

Get the bone data::

	for (SpineBone bone : animation.bones) {
	    // Update in-game objects with SRT from bone
	}

On destroy::

    animation.destroy();