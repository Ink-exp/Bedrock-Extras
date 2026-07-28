#include <android/log.h>
#include <vector>
#include <memory>

#include "module.hpp"
#include "module_culling.hpp"
#include "module_shulker.hpp"

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static std::vector<std::unique_ptr<IModule>> g_modules;
static bool g_initialized = false;

// Helper function to safely load our modules
static void init_bedrock_extras() {
    if (g_initialized) return;

    try {
        g_modules.push_back(std::make_unique<CullingModule>());
        g_modules.push_back(std::make_unique<BetterShulkerModule>());

        for (auto& mod : g_modules) {
            mod->on_enable();
        }

        g_initialized = true;
        LOGI("[Bedrock Extras] Successfully initialized all modules!");
    } catch (...) {
        LOGE("[Bedrock Extras] Error initializing modules.");
    }
}

// 1. Variadic PLGetModRegistration: Accept ANY parameters LeviLauncher passes without memory corruption
extern "C" __attribute__((visibility("default"))) void PLGetModRegistration(...) {
    LOGI("[Bedrock Extras] PLGetModRegistration called safely by LeviLauncher.");
    init_bedrock_extras();
}

// 2. Fallback constructor: Guarantees module init when dlopen() loads the .so
__attribute__((constructor))
void plugin_entry() {
    LOGI("[Bedrock Extras] Native library loaded via plugin_entry constructor.");
    init_bedrock_extras();
}
