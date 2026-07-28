#include <android/log.h>
#include <vector>
#include <memory>

#include "module.hpp"
#include "module_culling.hpp"
#include "module_shulker.hpp"

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static std::vector<std::unique_ptr<IModule>> g_modules;

extern "C" __attribute__((visibility("default"))) void PLGetModRegistration() {
    LOGI("[Bedrock Extras] Registered under LeviLauncher External Tab.");
}

__attribute__((constructor))
void plugin_entry() {
    LOGI("=============================================");
    LOGI("      BEDROCK EXTRAS V1.0 INITIALIZED        ");
    LOGI("=============================================");

    // Register active modules
    g_modules.push_back(std::make_unique<CullingModule>());
    g_modules.push_back(std::make_unique<BetterShulkerModule>());

    for (auto& mod : g_modules) {
        mod->on_enable();
    }
}
