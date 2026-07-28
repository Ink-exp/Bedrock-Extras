#include <android/log.h>
#include <dlfcn.h>
#include "culling.hpp"

#define LOG_TAG "CullingPlugin"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// 32 block render threshold (Squared: 32 * 32 = 1024)
static constexpr float MAX_DIST_SQ = 1024.0f; 

typedef void (*RenderEntityFn)(void* context, void* entity, Vector3* entity_pos, Vector3* camera_pos);
static RenderEntityFn g_orig_render_entity = nullptr;

extern "C" void hook_render_entity(void* context, void* entity, Vector3* entity_pos, Vector3* camera_pos) {
    // Fast drop if pointers are invalid to prevent frame stutters
    if (!entity_pos || !camera_pos) {
        if (g_orig_render_entity) g_orig_render_entity(context, entity, entity_pos, camera_pos);
        return;
    }

    // Fast squared Euclidean distance (No sqrt calls = Zero CPU overhead)
    float dx = entity_pos->x - camera_pos->x;
    float dy = entity_pos->y - camera_pos->y;
    float dz = entity_pos->z - camera_pos->z;

    // Skip draw call if entity is outside 32 blocks
    if ((dx * dx + dy * dy + dz * dz) > MAX_DIST_SQ) {
        return;
    }

    if (g_orig_render_entity) {
        g_orig_render_entity(context, entity, entity_pos, camera_pos);
    }
}

__attribute__((constructor))
void plugin_entry() {
    LOGI("Culling Plugin V2.0 Performance Mode initialized.");
}
