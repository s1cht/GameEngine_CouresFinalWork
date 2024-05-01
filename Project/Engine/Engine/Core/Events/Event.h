#pragma once

#include "IEvent.h"

#pragma region Classes

template<typename T>
class Event : public EventBase
{
public:
    using Function = std::function<void(const T&)>;
protected:
    std::vector<Function> handlers;
public:
    Event() = default;
public:
    size_t Connect(const Function&& func);
    void Disconnect(const size_t function) override;

    void Fire() const override;
    void Fire(const void* arg) const override;

    bool IsEmpty() override;;
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
public:
    size_t Connect(const Function&& func);
    void Disconnect(size_t function);

    void Fire() const override;
    void Fire(const void* arg) const override;

    bool IsEmpty() override;
};

#pragma endregion

#pragma region Implementation

template<typename T>
size_t Event<T>::Connect(const Function&& func) 
{
    handlers.emplace_back(std::move(func));
    return handlers.size() - 1;
}

template<typename T>
inline void Event<T>::Disconnect(const size_t function) 
{
    handlers.erase(handlers.begin() + function);
}

template<typename T>
inline void Event<T>::Fire() const {}

template<typename T>
inline void Event<T>::Fire(const void* arg) const 
{
    const T* typedArg = static_cast<const T*>(arg);
    for (const auto& handler : handlers) {
        handler(*typedArg);
    }
}

template<typename T>
inline bool Event<T>::IsEmpty() 
{ 
    return handlers.empty(); 
}

/*          -VOID-          */

inline size_t Event<void>::Connect(const Function&& func) 
{
    handlers.emplace_back(std::move(func));
    return handlers.size() - 1;
}

inline void Event<void>::Disconnect(size_t function) 
{
    handlers.erase(handlers.begin() + function);
}

inline void Event<void>::Fire() const 
{
    for (const auto& handler : handlers) 
    {
        handler();
    }
}

inline void Event<void>::Fire(const void* arg) const {}

inline bool Event<void>::IsEmpty() 
{ 
    return handlers.empty(); 
}

#pragma endregion