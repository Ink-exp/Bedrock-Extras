#pragma once
#include <dlfcn.h>
#include <android/log.h>
#include "dobby.h"
#include "gui.hpp"

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Hook signature for eglSwapBuffers (Renders HUD overlay)
typedef EGLBoolean (*eglSwapBuffers_t)(EGLDisplay dpy, EGLSurface surface);
static eglSwapBuffers_t orig_eglSwapBuffers = nullptr;

EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    GUI::draw_overlay();
    return orig_eglSwapBuffers(dpy, surface);
}

// Hook signature for ActorRenderDispatcher::render
typedef void (*ActorRender_t)(void* ctx, void* actor, void* pos, float param);
static ActorRender_t orig_ActorRender = nullptr;

void hook_ActorRender(void* ctx, void* actor, void* pos, float param) {
    if (actor != nullptr && g_cullingMod.is_enabled()) {
        // Read position pointer offset from actor struct in RAM
        float* coords = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(actor) + 0x148);
        
        // Example camera position reference
        if (g_cullingMod.should_cull_actor(coords[0], coords[1], coords[2], 0.0f, 64.0f, 0.0f)) {
            return; // Intercepts rendering pipeline (Culls distant entity)
        }
    }
    orig_ActorRender(ctx, actor, pos, param);
}

inline void install_hooks() {
    LOGI("[Bedrock Extras] Scanning libminecraftpe.so for symbol targets...");

    void* mc_handle = dlopen("libminecraftpe.so", RTLD_NOLOAD);
    if (!mc_handle) {
        LOGI("[Bedrock Extras] Waiting for libminecraftpe.so...");
        return;
    }

    // 1. Hook OpenGL Display Swap for HUD
    void* egl_sym = dlsym(RTLD_DEFAULT, "eglSwapBuffers");
    if (egl_sym) {
        DobbyHook(egl_sym, (dobby_dummy_func_t)hook_eglSwapBuffers, (dobby_dummy_func_t*)&orig_eglSwapBuffers);
        LOGI("[Bedrock Extras] Hooked eglSwapBuffers successfully!");
    }

    // 2. Hook Entity Render Pipeline
    void* render_sym = dlsym(mc_handle, "_ZN21ActorRenderDispatcher6renderER22ActorRenderContextR5ActorRK6Vec3fS6_");
    if (render_sym) {
        DobbyHook(render_sym, (dobby_dummy_func_t)hook_ActorRender, (dobby_dummy_func_t*)&orig_ActorRender);
        LOGI("[Bedrock Extras] Hooked ActorRenderDispatcher successfully!");
    }
}

