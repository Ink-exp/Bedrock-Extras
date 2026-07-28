#include <android/log.h>
#include <dlfcn.h>
#include <memory>
#include <vector>
#include "culling.hpp"

#define LOG_TAG "CullingPlugin"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Global culling parameters
static CullingConfig g_config;
static float g_max_distance_sq = 48.0f * 48.0f; // Pre-calculated (48 * 48 = 2304)

// Function pointer signature for entity rendering in Minecraft Bedrock
typedef void (*RenderEntityFn)(void* context, void* entity, Vector3* entity_pos, Vector3* camera_pos);
static RenderEntityFn g_orig_render_entity = nullptr;

// Primary Safe Culling Interceptor
extern "C" void hook_render_entity(void* context, void* entity, Vector3* entity_pos, Vector3* camera_pos) {
    // Safety check: if plugin is disabled or pointers are null, render normally
    if (!g_config.enabled || !entity_pos || !camera_pos) {
        if (g_orig_render_entity) {
            g_orig_render_entity(context, entity, entity_pos, camera_pos);
        }
        return;
    }

    // Distance calculation relative to camera
    float dist_sq = entity_pos->distance_squared_to(*camera_pos);

    // Skip drawing entity if outside configured render radius
    if (dist_sq > g_max_distance_sq) {
        return; // Early return skips the draw call (culls the entity)
    }

    // Render normally if within radius
    if (g_orig_render_entity) {
        g_orig_render_entity(context, entity, entity_pos, camera_pos);
    }
}

// LeviLauncher / Native Mod Loader compatibility symbol
extern "C" __attribute__((visibility("default"))) void PLGetModRegistration() {
    LOGI("[CullingPlugin] PLGetModRegistration called - Mod registered with launcher.");
}

// Automatic plugin entry constructor when shared library (.so) is loaded
__attribute__((constructor))
void plugin_entry() {
    LOGI("=============================================");
    LOGI("      SODIUM CULLING PLUGIN V1.0 LOADED      ");
    LOGI("  Target Radius: 48 Blocks (Performance Mode)");
    LOGI("=============================================");
}
