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

// LeviLauncher Native Mod Registration Entry point
extern "C" __attribute__((visibility("default"))) void PLGetModRegistration() {
    LOGI("[Bedrock Extras] PLGetModRegistration called by LeviLauncher.");
    
    if (g_initialized) return;

    try {
        // Safe module instantiation
        g_modules.push_back(std::make_unique<CullingModule>());
        g_modules.push_back(std::make_unique<BetterShulkerModule>());

        for (auto& mod : g_modules) {
            mod->on_enable();
        }

        g_initialized = true;
        LOGI("[Bedrock Extras] All modules successfully initialized!");
    } catch (const std::exception& e) {
        LOGE("[Bedrock Extras] Failed to register modules: %s", e.what());
    } catch (...) {
        LOGE("[Bedrock Extras] Unknown error occurred during mod initialization!");
    }
}

// Fallback constructor (Executes when .so is dlopen'd if loader skips PLGetModRegistration)
__attribute__((constructor))
void plugin_entry() {
    LOGI("[Bedrock Extras] Native library loaded into memory.");
}
