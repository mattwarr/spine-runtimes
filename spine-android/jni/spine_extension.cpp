#include <spine/spine.h>
#include <spine/extension.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <spine_context.h>

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {}
void _spAtlasPage_disposeTexture (spAtlasPage* self) {}

char* _spUtil_readFile (const char* path, int* length) {

	JNIEnv* env = (JNIEnv*) get_jni_env();
	AAssetManager* assetManager = (AAssetManager*) get_asset_manager();

	AAsset* asset = AAssetManager_open(assetManager, (const char *) path, AASSET_MODE_UNKNOWN);

	if (NULL == asset) {
		__android_log_print(ANDROID_LOG_ERROR, "SpineAndroid", "Asset (%s) not found", path);
		return NULL;
	}

	long size = AAsset_getLength(asset);

	*length = size;

	char* buffer = (char*) malloc (sizeof(char)*size);
	AAsset_read (asset,buffer,size);
	AAsset_close(asset);

	return buffer;
}
