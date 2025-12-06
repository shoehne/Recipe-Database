#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Core.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

#include "imgui.h"

namespace Recipe_Database{

	class Window;

	// Main properties of the window (title, width, height)
	struct WindowProps {

		WindowProps(const std::string& name = "",
			uint32_t x = 0,
			uint32_t y = 0,
			uint32_t w = 1280,
			uint32_t h = 720,
			Recipe_Database::Window* p = nullptr)
			: title(name), x_pos(x), y_pos(y), width(w), height(h), parent(p) {}

		std::string title;
		uint32_t x_pos;
		uint32_t y_pos;
		uint32_t width;
		uint32_t height;
		Recipe_Database::Window* parent;
	};

	struct WindowData {

		WindowData(const std::string& name = "",
			uint32_t x = 0,
			uint32_t y = 0,
			uint32_t w = 0,
			uint32_t h = 0,
			Recipe_Database::Window* p = nullptr) 
		: title(name), x_pos(x), y_pos(y), width(w), height(h), parent(p) {}

		using EventCallbackFn = std::function<void(Event&)>;

		std::string title;
		uint32_t x_pos, y_pos, height, width;
		Window* parent;

		EventCallbackFn event_callback;
	};

	class Window {

	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual void* GetParentWindow() const = 0;
		virtual void Init(const WindowProps& props) = 0;
		virtual void OnUpdate() = 0;
		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static Ref<Window> Create(const WindowProps& props = WindowProps());
	};
}

#endif // !WINDOW_H

