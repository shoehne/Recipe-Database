//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "Platform/Windows/WindowWindows.h"

#include "examples/imgui_impl_win32.h"

// Windows callback function
LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Recipe_Database::WindowWindows::WindowWindows(const WindowProps& props) {

	
}

Recipe_Database::WindowWindows::~WindowWindows() {

	Shutdown();
}

uint32_t Recipe_Database::WindowWindows::GetHeight() const {

	return window_data->height;
}

uint32_t Recipe_Database::WindowWindows::GetWidth() const {

	return window_data->width;
}

void Recipe_Database::WindowWindows::SetEventCallback(const EventCallbackFn& callback) {

	window_data->event_callback = callback;
}

void Recipe_Database::WindowWindows::Init(const WindowProps& props) {

	// Create a new WindowData object.
	window_data = new (std::nothrow) WindowData;
	if (window_data == NULL) {

		return;
	}

	// Assume that the created window has no parent.
	HWND parent = NULL;
	// Check whether window_data.parent points to an empty address.
	// If it points to a valid object assign the stored window handle to HWND parent.
	if (window_data->parent != nullptr) {

		parent = (HWND)window_data->parent->GetNativeWindow();
	}

	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Recipe Database";

	WNDCLASS window_class = {};

	window_class.lpfnWndProc = WindowProc;
	window_class.hInstance = GetModuleHandle(0);
	window_class.lpszClassName = CLASS_NAME;

	RegisterClass(&window_class);

	// Create the window
	window_handle = CreateWindowEx(0,
		CLASS_NAME,
		Recipe_Database::ToWstring(window_data->title).c_str(),
		WS_OVERLAPPEDWINDOW,
		// Initial position and size of the window.
		window_data->x_pos,
		window_data->y_pos,
		window_data->width,
		window_data->height,
		parent,
		NULL,
		GetModuleHandle(0),
		window_data);

	if (window_handle == NULL) {

		RECIPE_DATABASE_ERROR("ERROR: Failed to create window!");
		return;
	}

	ShowWindow(window_handle,
		SW_SHOW);
	UpdateWindow(window_handle);
}

void Recipe_Database::WindowWindows::OnUpdate() {

	MSG message = {};

	// Peek for a message without blocking the thread.
	bool peek = PeekMessage(&message,
		window_handle,
		0,
		0,
		PM_REMOVE);
	
	// If peek equals 0 then there is no message currently available.
	// In that case just exit without trying to translate and dispatch a message.
	if (peek == 0) {
	
		return;
	}
	
	TranslateMessage(&message);
	DispatchMessage(&message);
}

void Recipe_Database::WindowWindows::Shutdown() {

	PostQuitMessage(0);
}

LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {

	Recipe_Database::WindowData* data;
	if (uMsg == WM_CREATE) {

		CREATESTRUCT* p_create = reinterpret_cast<CREATESTRUCT*>(lParam);
		data = reinterpret_cast<Recipe_Database::WindowData*>(p_create->lpCreateParams);
		SetWindowLongPtr(hwnd,
			GWLP_USERDATA,
			(LONG_PTR)data);
	}
	else {

		LONG_PTR ptr = GetWindowLongPtr(hwnd,
			GWLP_USERDATA);
		data = reinterpret_cast<Recipe_Database::WindowData*>(ptr);
	}

	if (ImGui_ImplWin32_WndProcHandler(hwnd, 
		uMsg,
		wParam,
		lParam)) {

		return true;
	}

	switch (uMsg) {

		case WM_CLOSE: {

			Recipe_Database::WindowCloseEvent event;
			data->event_callback(event);
		}
		case WM_SIZE: {


		}
	}

	return DefWindowProc(hwnd,
		uMsg,
		wParam,
		lParam);
}
