Simple Android Spine Runtime
----------------------------

For the project I am working on I needed a simplified version of a spine runtime that did not depend on any external
rendering engines and simply allowed me to load and run an animation from spine with just raw SRT
(Scale, Rotation, Transformation) data reported for the bones in the animation.

This implementation leverages the spine-c implementation and provides a simple JNI wrapper for use in Android.

The JSON file produced by spline simply needs to be placed in the **assets** folder of your android app.

 .. note::

    This library is limited to running simple animations and is not a full spine runtime.

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

    animation.setAnimation(0, "jump", false);

Optionally add an animation to the track::

    animation.addAnimation(0, "walk", true, 0);

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

On destroy clean up::

    animation.destroy();