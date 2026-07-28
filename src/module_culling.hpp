#pragma once
#include "module.hpp"
#include <android/log.h>
#include <cmath>

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

struct Vector3 {
    float x;
    float y;
    float z;

    float distance_squared_to(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return (dx * dx) + (dy * dy) + (dz * dz);
    }
};

class CullingModule : public IModule {
private:
    bool enabled = false;
    float max_distance_sq = 40.0f * 40.0f; // 40 block render radius

public:
    std::string get_name() const override { return "Sodium Culling"; }

    void on_enable() override {
        enabled = true;
        LOGI("[Bedrock Extras] Enabled Sodium Culling Module!");
    }

    void on_disable() override {
        enabled = false;
        LOGI("[Bedrock Extras] Disabled Sodium Culling Module!");
    }

    bool is_enabled() const { return enabled; }

    bool should_cull(const Vector3& entity_pos, const Vector3& camera_pos) const {
        if (!enabled) return false;
        return entity_pos.distance_squared_to(camera_pos) > max_distance_sq;
    }
};

