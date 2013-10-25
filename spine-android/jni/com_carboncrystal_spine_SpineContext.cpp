#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <com_carboncrystal_spine_SpineContext.h>
#include <spine_context.h>

static AAssetManager* assetManager;

JNIEXPORT jlong JNICALL Java_com_carboncrystal_spine_SpineContext_init(JNIEnv *e, jclass caller, jobject jAssetManager) {
	assetManager = (AAssetManager*) AAssetManager_fromJava(e, jAssetManager);
	return (jlong) NULL;
}

AAssetManager* get_asset_manager() {
	return assetManager;
}
