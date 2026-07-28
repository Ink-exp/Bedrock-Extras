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

static void init_modules() {
    if (g_initialized) return;
    g_initialized = true;

    g_modules.push_back(std::make_unique<CullingModule>());
    g_modules.push_back(std::make_unique<BetterShulkerModule>());

    for (auto& mod : g_modules) {
        mod->on_enable();
    }
    LOGI("[Bedrock Extras] Modules initialized successfully!");
}

// 1. Standard JNI OnLoad hook (called when Java loads the shared library)
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("[Bedrock Extras] JNI_OnLoad triggered.");
    init_modules();
    return JNI_VERSION_1_6;
}

// 2. Export PLGetModRegistration safely returning 0 / nullptr to satisfy createLoadedModEntry
extern "C" __attribute__((visibility("default"))) void* PLGetModRegistration(void* a1, void* a2, void* a3) {
    LOGI("[Bedrock Extras] PLGetModRegistration called safely.");
    init_modules();
    return nullptr;
}
