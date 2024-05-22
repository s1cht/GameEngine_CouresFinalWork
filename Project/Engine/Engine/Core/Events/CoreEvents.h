#pragma once
#include "pch.h"
#include "Event.h"

class CoreEvents
{
private:
    std::map<std::string, std::map<std::string, std::shared_ptr<EventBase>>> Events;

public:
    CoreEvents();
    ~CoreEvents();

public:
    template<typename T>
    size_t AddListener(const std::function<void(const ENG_PTR<T>&)>& function, const std::string& eventName)
    {
        auto& innerMap = Events[typeid(T).name()];

        auto it = innerMap.find(eventName);
        if (it != innerMap.end()) 
        {
            auto event = std::dynamic_pointer_cast<Event<T>>(it->second);
            if (!event) 
                return size_t(-1);
            return event->Connect(std::move(function));
        }
        else 
        {
            auto newEvent = std::make_shared<Event<T>>();
            size_t index = newEvent->Connect(std::move(function));
            innerMap[eventName] = newEvent;
            return index;
        }
    }
    size_t AddListener(const std::function<void()>& function, const std::string& eventName);

    void FireEvent(std::string);
    void FireEvent(std::string eventName, ENG_PTR<void> arg);
private:
    void DeleteListener(std::string, size_t);
public:
    friend class Window;
    friend class GameEngine;

};

extern std::unique_ptr<CoreEvents> EngineCoreEvents;