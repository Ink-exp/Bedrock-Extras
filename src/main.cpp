#include <android/log.h>
#include <dlfcn.h>
#include "culling.hpp"

#define LOG_TAG "CullingPlugin"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// 48 blocks distance (squared to avoid heavy CPU math)
static float g_max_distance_sq = 48.0f * 48.0f; 

// The signature for the game's render function
typedef void (*RenderEntityFn)(void* context, void* entity, Vector3* entity_pos, Vector3* camera_pos);
static RenderEntityFn g_orig_render_entity = nullptr;

// THE V2.0 SAFE CULLING HOOK (extern "C" prevents C++ name mangling for the launcher)
extern "C" void hook_render_entity(void* context, void* entity, Vector3* entity_pos, Vector3* camera_pos) {
    // 1. Safety Net: If the game fails to provide a position, just render it normally to prevent crashes
    if (!entity_pos || !camera_pos) {
        if (g_orig_render_entity) {
            g_orig_render_entity(context, entity, entity_pos, camera_pos);
        }
        return;
    }

    // 2. High-Speed Math: Calculate distance from the player's ACTUAL camera
    float dx = entity_pos->x - camera_pos->x;
    float dy = entity_pos->y - camera_pos->y;
    float dz = entity_pos->z - camera_pos->z;
    float dist_sq = (dx * dx) + (dy * dy) + (dz * dz);

    // 3. If the entity is further than 48 blocks, skip drawing it completely!
    if (dist_sq > g_max_distance_sq) {
        return; 
    }

    // 4. Otherwise, it is close enough to see, so draw it
    if (g_orig_render_entity) {
        g_orig_render_entity(context, entity, entity_pos, camera_pos);
    }
}

// Automatically runs when LeviLauncher injects the mod
__attribute__((constructor))
void plugin_entry() {
    LOGI("=============================================");
    LOGI("  CULLING PLUGIN V2.0 - LOADED SUCCESSFULLY  ");
    LOGI("  LeviLauncher Mod Manager Integration Active ");
    LOGI("=============================================");
}
