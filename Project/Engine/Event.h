#pragma once
#include "pch.h"

class Event
{
public:
	using Function = std::function<void()>;
protected:
    std::vector<Function> handlers;
public:
    virtual size_t Connect(const Function& handler) = 0;
    virtual void Disconnect(const size_t function) = 0;
    virtual void Fire() const = 0;
};