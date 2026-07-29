#include <jni.h>
#include <android/log.h>
#include <vector>
#include <memory>

#include "module.hpp"
#include "module_culling.hpp"
#include "module_shulker.hpp"

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static std::vector<std::unique_ptr<IModule>> g_modules;
static bool g_initialized = false;

static void init_bedrock_extras() {
    if (g_initialized) return;
    g_initialized = true;

    LOGI("=============================================");
    LOGI("      BEDROCK EXTRAS V1.0 INITIALIZED        ");
    LOGI("=============================================");

    g_modules.push_back(std::make_unique<CullingModule>());
    g_modules.push_back(std::make_unique<BetterShulkerModule>());

    for (auto& mod : g_modules) {
        mod->on_enable();
    }
}

// JNI_OnLoad triggers safely when Java calls System.loadLibrary() / dlopen()
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("[Bedrock Extras] Loaded via JNI_OnLoad.");
    init_bedrock_extras();
    return JNI_VERSION_1_6;
}

// C++ Constructor triggers immediately upon dlopen()
__attribute__((constructor))
void plugin_entry() {
    LOGI("[Bedrock Extras] Loaded via plugin_entry constructor.");
    init_bedrock_extras();
}
