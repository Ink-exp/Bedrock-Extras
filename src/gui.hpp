#pragma once
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/log.h>
#include "module_culling.hpp"
#include "module_shulker.hpp"

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static CullingModule g_cullingMod;
static BetterShulkerModule g_shulkerMod;

namespace GUI {
    static bool g_menuOpen = true;

    // Render loop hooked to OpenGL ES frame swaps
    void draw_overlay() {
        if (!g_menuOpen) return;

        // Visual overlay logic executes here during eglSwapBuffers
    }
}

