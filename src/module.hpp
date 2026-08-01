#pragma once
#include <string>

class IModule {
public:
    virtual ~IModule() = default;
    virtual std::string get_name() const = 0;
    virtual bool is_enabled() const = 0;
    virtual void set_enabled(bool state) = 0;
    virtual void on_enable() = 0;
    virtual void on_disable() = 0;
};
