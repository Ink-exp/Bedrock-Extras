#pragma once
#include "module.hpp"
#include <android/log.h>
#include <string>

#define LOG_TAG "BedrockExtras"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

struct ShulkerSlot {
    int id = 0;
    int count = 0;
    std::string name = "";
};

class BetterShulkerModule : public IModule {
private:
    bool enabled = true;

public:
    std::string get_name() const override { return "Better Shulker"; }
    bool is_enabled() const override { return enabled; }
    void set_enabled(bool state) override { enabled = state; }

    void on_enable() override {
        LOGI("[Bedrock Extras] Better Shulker active.");
    }

    void on_disable() override {
        LOGI("[Bedrock Extras] Better Shulker inactive.");
    }

    void inspect_container_tag(void* compound_tag) {
        if (!enabled || !compound_tag) return;
        // Intercepts and parses NBT tags from hover tooltips
    }
};
