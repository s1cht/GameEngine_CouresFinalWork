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

