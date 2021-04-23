#pragma once
#ifndef WINDOWWINDOWS_H
#define WINDOWWINDOWS_H

#include "Core/Window/Window.h"

namespace Recipe_Database {

	struct WindowData {

		using EventCallbackFn = std::function<void(Event&)>;

		std::wstring title;
		uint32_t x_pos, y_pos, height, width;
		Window* parent;

		EventCallbackFn event_callback;
	};

	class WindowWindows : public Window {

	public:
		WindowWindows(const WindowProps& props);
		virtual ~WindowWindows();

		uint32_t GetHeight() const override;
		virtual void* GetNativeWindow() const override { return window_handle; };
		uint32_t GetWidth() const override;
		virtual void OnUpdate() override;
		void SetEventCallback(const EventCallbackFn& callback) override;

	private:
		
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		HWND window_handle;
		WindowData* window_data;
	};
}

#endif // !WINDOWWINDOWS_H

