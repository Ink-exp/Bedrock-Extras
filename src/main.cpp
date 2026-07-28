#include <android/log.h>
#include <dlfcn.h>
#include <cmath>
#include "culling.hpp"

#define LOG_TAG "CullingPlugin"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Configurable render thresholds
static float g_max_distance_sq = 48.0f * 48.0f; // 48 block radius squared

typedef void (*RenderEntityFn)(void* context, void* entity, Vector3* entity_pos, Vector3* camera_pos);
static RenderEntityFn g_orig_render_entity = nullptr;

// Safe Entity Culling Hook
void hook_render_entity(void* context, void* entity, Vector3* entity_pos, Vector3* camera_pos) {
    // 1. Safety Check: Never cull if positions are invalid
    if (!entity_pos || !camera_pos) {
        if (g_orig_render_entity) g_orig_render_entity(context, entity, entity_pos, camera_pos);
        return;
    }

    // 2. Calculate true distance relative to the CAMERA, not world origin (0,0)
    float dx = entity_pos->x - camera_pos->x;
    float dy = entity_pos->y - camera_pos->y;
    float dz = entity_pos->z - camera_pos->z;
    float dist_sq = (dx * dx) + (dy * dy) + (dz * dz);

    // 3. Cull only if entity is beyond radius
    if (dist_sq > g_max_distance_sq) {
        return; // Skip rendering off-screen / distant entity
    }

    // 4. Render normally
    if (g_orig_render_entity) {
        g_orig_render_entity(context, entity, entity_pos, camera_pos);
    }
}

__attribute__((constructor))
void plugin_entry() {
    LOGI("Culling Plugin v2.0 (Stable Release) initialized.");
}
