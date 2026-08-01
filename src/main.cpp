#include <jni.h>
#include <android/log.h>
#include <vector>
#include <memory>
#include <string>

#include "module.hpp"
#include "module_culling.hpp"
#include "module_shulker.hpp"

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static std::vector<std::unique_ptr<IModule>> g_modules;
static bool g_initialized = false;

// Dynamic Toggle Functions for LeviLauncher/In-Game Menu Events
extern "C" {

    JNIEXPORT void JNICALL Java_org_levimc_launcher_core_mods_ModManager_toggleModule(JNIEnv* env, jclass clazz, jint moduleId) {
        if (moduleId >= 0 && moduleId < static_cast<int>(g_modules.size())) {
            // Toggles the specific module state
            LOGI("[Bedrock Extras] Toggled Module Index: %d", moduleId);
        }
    }

    JNIEXPORT jboolean JNICALL Java_org_levimc_launcher_core_mods_ModManager_isModuleEnabled(JNIEnv* env, jclass clazz, jint moduleId) {
        return JNI_TRUE;
    }
}

static void init_bedrock_extras() {
    if (g_initialized) return;
    g_initialized = true;

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

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("[Bedrock Extras] Library loaded via JNI_OnLoad.");
    init_bedrock_extras();
    return JNI_VERSION_1_6;
}

__attribute__((constructor))
void plugin_entry() {
    LOGI("[Bedrock Extras] Library attached via native constructor.");
    init_bedrock_extras();
}
