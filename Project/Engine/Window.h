#pragma once
#include "pch.h"
#include "Event.h"

const LPCWSTR winClass = L"Engine";

class Window
{
private:
	HWND m_hwnd;
	int width, height;
	bool isRunning = false;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	class WindowDestroyedEvent : public Event
	{
	public:
		~WindowDestroyedEvent() {};
	public:
		size_t Connect(const Function& handler) override
		{
			handlers.push_back(handler);
			return handlers.size();
		}
		void Disconnect(const size_t function) override
		{
			handlers.erase(handlers.begin() + function);
		}
		void Fire() const override
		{
			for (const auto& function : handlers) {
				function();
			}
		}
	};
public:
	Window(INT w, INT h) : width(w), height(h), m_hwnd(HWND()) {};
	bool Create(LPCWSTR title);
	bool Runs();
	~Window();
public:
	//virtual void onDestroy(); -- DEPRECATED
	WindowDestroyedEvent destroy;
};

