#pragma once
#include <string>

class IModule {
public:
    virtual ~IModule() = default;
    virtual std::string get_name() const = 0;
    virtual void on_enable() = 0;
    virtual void on_disable() = 0;
    virtual void on_render() {}
};

