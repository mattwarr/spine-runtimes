Simple Android Spine Runtime
----------------------------

For the project I am working on I needed a simplified version of a spine runtime that did not depend on any external
rendering engines and simply allowed me to load and run an animation from spine with just raw SRT
(Scale, Rotation, Transformation) data reported for the bones in the animation.

This implementation leverages the spine-c implementation and provides a simple JNI wrapper for use in Android.

The JSON files produced by spline simply needs to be placed in the **assets** folder of your android app.

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

	SpineAnimationFactory factory = new SpineAnimationFactory(this, "spineboy.atlas", "spineboy.json", 1);


Animation data is written to a native buffer, so we need to allocate a buffer and specify the offset/stride.
We do this via a listener so we know how many bones are in the animation::

    final BasicSpineVertexBufferInfo vbi = new BasicSpineVertexBufferInfo();

    static final int BYTES_PER_FLOAT = 4;
	static final int FLOATS_PER_BONE = 12; // 6 x 2 coords for GL_TRIANGLES

    SpineAnimationListener listener = new SpineAnimationListener() {
        @Override
        public void onCreateBone(SpineBone bone) {
            // This will be called for every bone created
        }

        @Override
        public void onCreateSkeleton(int numBones) {

            // Set the draw mode, offset and stride for this animation
            // NOTE: Only GL_TRIANGLES is currently supported.

            vbi.setDrawMode(GLES20.GL_TRIANGLES);
            vbi.setOffset(0);
            vbi.setStride(0);

            // Allocate a native buffer
            vbi.setVertexBuffer(ByteBuffer.allocateDirect(BYTES_PER_FLOAT * FLOATS_PER_BONE * numBones).order(ByteOrder.nativeOrder()).asFloatBuffer());
        }
    };

Now we can create the animation with the vertex buffer info::

	SpineAnimation animation = factory.create(listener, vbi);

Set the animation and track::

    animation.setAnimation(0, "jump", false);

Optionally add an animation to the track::

    animation.addAnimation(0, "walk", true, 0);

Step the animation::

	animation.step(delta); // delta is in milliseconds

Get the bone data::

	for (SpineBone bone : animation.bones) {
	    // Update in-game objects with SRT from bone
	}

On destroy clean up::

    factory.destroy();
