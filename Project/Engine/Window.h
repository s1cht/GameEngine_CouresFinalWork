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

		void Fire(...) const override 
		{
			for (const auto& function : handlers) {
				function();
			}
		};
	};
	class WindowUpdatedEvent : public Event
	{
	public:
		using Function = std::function<void(RECT)>;

	private:
		std::vector<Function> handlers;
	public:
		~WindowUpdatedEvent() {};

		void Fire(RECT rc) const
		{
			for (const auto& function : handlers) {
				function(rc);
			}
		};
	};
public:
	Window(INT w, INT h) : width(w), height(h), m_hwnd(HWND()) {};
	bool Create(LPCWSTR title);
	bool Runs();
	~Window();
public:
	//virtual void onDestroy(); -- DEPRECATED
	WindowDestroyedEvent destroy;
	WindowUpdatedEvent update;
};

