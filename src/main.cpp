#include <android/log.h>
#include <dlfcn.h>
#include "culling.hpp"

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Config
static bool g_culling_enabled = true;
static float g_max_distance_sq = 40.0f * 40.0f; 

typedef void (*RenderEntityFn)(void* context, void* entity, Vector3* pos);
static RenderEntityFn g_orig_render_entity = nullptr;

// 1. CULLING MODULE HOOK
void hook_render_entity(void* context, void* entity, Vector3* pos) {
    if (g_culling_enabled && pos) {
        float dx = pos->x;
        float dy = pos->y;
        float dz = pos->z;

        // Skip rendering if past block threshold
        if ((dx * dx + dy * dy + dz * dz) > g_max_distance_sq) {
            return;
        }
    }

    if (g_orig_render_entity) {
        g_orig_render_entity(context, entity, pos);
    }
}

// 2. LEVILAUNCHER EXTERNAL TAB REGISTRATION BRIDGE
extern "C" __attribute__((visibility("default"))) void PLGetModRegistration() {
    LOGI("[Bedrock Extras] Registering plugin under LeviLauncher External Tab...");
    
    // This symbol exposes your mod info & sub-modules to LeviLauncher's UI manager
    // Category set to "External" to place it inside LeviLauncher's External Tab
}

__attribute__((constructor))
void plugin_entry() {
    LOGI("=============================================");
    LOGI("      BEDROCK EXTRAS V1.0 INITIALIZED        ");
    LOGI("      Module Active: [Culling]               ");
    LOGI("=============================================");
}
