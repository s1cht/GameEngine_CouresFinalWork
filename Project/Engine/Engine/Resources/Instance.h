#pragma once
#include "pch.h"
#include "Engine/Core/Events/Event.h"

using namespace std;

class Instance
{
public:
	Instance();
	~Instance();
public:
	template<typename className> 
	Instance* New();
	template<typename className> 
	Instance* New(Instance* parent);

public:
	virtual ~Instance() = default;

	virtual void Destroy() = 0;
	virtual bool SetParent(Instance* parent);

protected:
	wstring Name;
	Instance* Parent;
	Event<void> OnDestroyed;
};

template<typename className>
Instance* Instance::New()
{
	className* instance;
	Instance* temp;

	instance = new className;
	temp = dynamic_cast<Instance*>(&instance);

	if (!temp)
		return nullptr;

	delete temp;
	return instance;
}

template<typename className>
Instance* Instance::New(Instance* parent)
{
	className* instance;
	Instance* temp;

	instance = new className;
	temp = dynamic_cast<Instance*>(&instance);

	if (!temp)
		return nullptr;

	delete temp;

	instance->SetParent(parent);

	return instance;
}
