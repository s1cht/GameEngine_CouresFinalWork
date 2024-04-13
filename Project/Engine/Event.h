#pragma once
#include "pch.h"


class Event
{
public:
	using Function = std::function<void()>;
protected:
    std::vector<Function*> handlers;
public:
    template<typename Func, typename... Args>
    size_t Connect(Func&& func) {
        handlers.emplace_back(func);
        return handlers.size() - 1;
    }
	virtual void Disconnect(const size_t function)
	{
		handlers.erase(handlers.begin() + function);
	}
	virtual void Fire(...) const {};
};