#pragma once
#include "pch.h"

class EventBase
{
public:
    virtual ~EventBase() = default;
    virtual void Disconnect(size_t function) = 0;
    virtual void Fire() const = 0;
    virtual void Fire(const void* arg) const = 0;
    virtual bool IsEmpty() = 0;
};

template<typename T>    
class Event : public EventBase
{
public:
    using Function = std::function<void(const T&)>;
protected:
    std::vector<Function> handlers;
public:
    Event() = default;
    size_t Connect(const Function&& func) {
        handlers.emplace_back(std::move(func));
        return handlers.size() - 1;
    }
    void Disconnect(const size_t function) override {
        handlers.erase(handlers.begin() + function);
    }
    void Fire() const override {}

    void Fire(const void* arg) const override {
        const T* typedArg = static_cast<const T*>(arg);
        for (const auto& handler : handlers) {
            handler(*typedArg);
        }
    }
    bool IsEmpty() override { return handlers.empty(); };
};

template<>
class Event<void> : public EventBase
{
public:
    using Function = std::function<void()>;

private:
    std::vector<Function> handlers;

public:
    Event() = default;

    size_t Connect(const Function&& func) {
        handlers.emplace_back(std::move(func));
        return handlers.size() - 1;
    }

    void Disconnect(size_t function) {
        handlers.erase(handlers.begin() + function);
    }

    void Fire() const override {
        for (const auto& handler : handlers) {
            handler();
        }
    }

    void Fire(const void* arg) const override {}

    bool IsEmpty() override { return handlers.empty(); }
};