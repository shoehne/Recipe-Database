#pragma once
#ifndef WINDOWWINDOWS_H
#define WINDOWWINDOWS_H

#include "Core/Window.h"

namespace Recipe_Database {

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
		
		void CleanUpDeviceD3D();
		void CleanUpRenderTarget();
		bool CreateDeviceD3D(HWND hwnd);
		void CreateRenderTarget();
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		HWND window_handle;
		WindowData* window_data;
	};
}

#endif // !WINDOWWINDOWS_H

