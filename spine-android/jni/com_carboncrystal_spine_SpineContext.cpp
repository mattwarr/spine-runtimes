#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <com_carboncrystal_spine_SpineContext.h>
#include <SpineAnimation.h>
#include <SpineCallback.h>
#include <spine_context.h>

static JNIEnv* env;
static AAssetManager* assetManager;

JNIEXPORT void JNICALL Java_com_carboncrystal_spine_SpineContext_init(JNIEnv *e, jclass caller, jobject jAssetManager) {
	env = e;
	assetManager = (AAssetManager*) AAssetManager_fromJava(env, jAssetManager);
}

JNIEnv* get_jni_env() {
	return env;
}

AAssetManager* get_asset_manager() {
	return assetManager;
}

