#pragma once
#include "module.hpp"
#include <android/log.h>

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

class CullingModule : public IModule {
private:
    bool enabled = true;
    float max_distance_sq = 36.0f * 36.0f; // 36 block culling radius

public:
    std::string get_name() const override { return "Sodium Culling"; }
    bool is_enabled() const override { return enabled; }
    void set_enabled(bool state) override { enabled = state; }

    void on_enable() override {
        LOGI("[Bedrock Extras] Culling logic activated.");
    }

    void on_disable() override {
        LOGI("[Bedrock Extras] Culling logic deactivated.");
    }

    // Called inside the ActorRenderDispatcher hook
    bool should_cull_actor(float actorX, float actorY, float actorZ, float camX, float camY, float camZ) {
        if (!enabled) return false;
        float dx = actorX - camX;
        float dy = actorY - camY;
        float dz = actorZ - camZ;
        return ((dx * dx) + (dy * dy) + (dz * dz)) > max_distance_sq;
    }
};
