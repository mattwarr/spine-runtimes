#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#ifdef __cplusplus
extern "C"
#endif

JavaVM* get_jni_env();

AAssetManager* get_asset_manager();
