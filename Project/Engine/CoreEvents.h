#pragma once
#include "pch.h"
#include "Event.h"

class CoreEvents
{
private:
	std::map<std::string, Event*> Events;
public:
	CoreEvents();
	~CoreEvents();
public:
	void RegisterCoreEvent(std::string, Event*);
	size_t AddListener(const Event::Function&, std::string);
	void DeleteListener(std::string, size_t);
};


extern CoreEvents EngineCoreEvents;