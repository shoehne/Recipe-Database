#pragma once
#ifndef WINDOWWINDOWS_H
#define WINDOWWINDOWS_H

#include "Core/Window.h"

//#include "imgui.h"
#include "examples/imgui_impl_dx11.h"
#include "examples/imgui_impl_win32.h"

namespace Recipe_Database {

	class WindowWindows : public Window {

	public:
		WindowWindows(const WindowProps& props);
		virtual ~WindowWindows();

		uint32_t GetHeight() const override;
		virtual void* GetNativeWindow() const override { return window_handle; };
		virtual void* GetParentWindow() const override { return window_data->parent; };
		uint32_t GetWidth() const override;
		virtual void OnUpdate() override;
		void SetEventCallback(const EventCallbackFn& callback) override;

	private:
		
		void CleanUpDeviceD3D();
		void CleanUpRenderTarget();
		bool CreateDeviceD3D(HWND hwnd);
		void CreateRenderTarget();
		virtual void Init(const WindowProps& props) override;
		virtual void Shutdown();

		ImGuiContext* imgui_context;
		ImGuiIO io;
		HWND window_handle;
		WindowData* window_data;
	};
}

#endif // !WINDOWWINDOWS_H

