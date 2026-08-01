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
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static std::vector<std::unique_ptr<IModule>> g_modules;
static bool g_initialized = false;

// Safe internal initialization logic
static void init_bedrock_extras() {
    if (g_initialized) return;
    g_initialized = true;

    LOGI("=============================================");
    LOGI("      BEDROCK EXTRAS INITIALIZATION         ");
    LOGI("=============================================");

    try {
        g_modules.push_back(std::make_unique<CullingModule>());
        g_modules.push_back(std::make_unique<BetterShulkerModule>());

        for (auto& mod : g_modules) {
            mod->on_enable();
        }
        LOGI("[Bedrock Extras] All modules registered successfully.");
    } catch (const std::exception& e) {
        LOGE("[Bedrock Extras] Failed to register modules: %s", e.what());
    } catch (...) {
        LOGE("[Bedrock Extras] Unknown error initializing modules.");
    }
}

// Native JNI export hooks to allow LeviLauncher's GUI to query and toggle settings
extern "C" {

    JNIEXPORT jboolean JNICALL Java_org_levimc_launcher_core_mods_ModManager_isModuleActive(JNIEnv* env, jclass clazz, jint index) {
        if (index >= 0 && index < static_cast<jint>(g_modules.size())) {
            return JNI_TRUE;
        }
        return JNI_FALSE;
    }

    JNIEXPORT void JNICALL Java_org_levimc_launcher_core_mods_ModManager_toggleModuleState(JNIEnv* env, jclass clazz, jint index) {
        LOGI("[Bedrock Extras] Toggling module at index %d", index);
        if (index >= 0 && index < static_cast<jint>(g_modules.size())) {
            // Module toggle logic execution
        }
    }

    // Variadic loader hook to satisfy libpreloader.so
    __attribute__((visibility("default"))) void* PLGetModRegistration(...) {
        LOGI("[Bedrock Extras] PLGetModRegistration safely called.");
        init_bedrock_extras();
        return nullptr;
    }
}

// JNI_OnLoad called during Java System.loadLibrary()
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("[Bedrock Extras] Attached via JNI_OnLoad.");
    init_bedrock_extras();
    return JNI_VERSION_1_6;
}

// Native constructor invoked immediately upon dlopen()
__attribute__((constructor))
void plugin_entry() {
    LOGI("[Bedrock Extras] Attached via native constructor.");
    init_bedrock_extras();
}
