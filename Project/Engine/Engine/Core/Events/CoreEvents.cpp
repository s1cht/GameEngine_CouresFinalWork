#include "pch.h"
#include "CoreEvents.h"

CoreEvents::CoreEvents() : Events({}) {}

CoreEvents::~CoreEvents() {}

void CoreEvents::FireEvent(std::string eventName) {
    for (auto& typeMap : Events) {
        auto& innerMap = typeMap.second;
        auto it = innerMap.find(eventName);
        if (it != innerMap.end()) {
            it->second->Fire();
            return;
        }
    }
}

void CoreEvents::FireEvent(std::string eventName, ENG_PTR<void> arg) {
    for (auto& typeMap : Events) {
        auto& innerMap = typeMap.second;
        auto it = innerMap.find(eventName);
        if (it != innerMap.end()) {
            it->second->Fire(arg);
            return;
        }
    }
}
void CoreEvents::DeleteListener(std::string eventName, size_t function)
{
    for (auto& typeMap : Events) {
        auto& innerMap = typeMap.second;
        auto it = innerMap.find(eventName);
        if (it != innerMap.end()) {
            it->second->Disconnect(function);
            if (it->second->IsEmpty()) {
                innerMap.erase(it);
            }
            return;
        }
    }
}

size_t CoreEvents::AddListener(const std::function<void()>& function, const std::string& eventName)
{
    auto& innerMap = Events[typeid(void).name()];

    auto it = innerMap.find(eventName);
    if (it != innerMap.end()) {
        auto event = std::dynamic_pointer_cast<Event<void>>(it->second);
        if (!event) {
            return size_t(-1);
        }
        return event->Connect(std::move(function));
    }
    else {
        auto newEvent = std::make_shared<Event<void>>();
        size_t index = newEvent->Connect(std::move(function));
        innerMap[eventName] = newEvent;
        return index;
    }
}

std::unique_ptr<CoreEvents> EngineCoreEvents = std::make_unique<CoreEvents>();