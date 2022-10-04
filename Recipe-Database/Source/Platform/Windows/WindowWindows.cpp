//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "Platform/Windows/WindowWindows.h"
#include "Platform/Windows/ImGuiD3D11Impl.h"
#include "Platform/Windows/ImGuiWin32Impl.h"

#include "imgui.h"

// Windows callback function
LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Data
static ID3D11Device* d3d_device = NULL;
static ID3D11DeviceContext* d3d_device_context = NULL;
static IDXGISwapChain* swap_chain = NULL;
static ID3D11RenderTargetView* main_render_target_view = NULL;

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

void Recipe_Database::WindowWindows::SetEventCallback(const EventCallbackFn& callback) {

	window_data->event_callback = callback;
}

void Recipe_Database::WindowWindows::CleanUpDeviceD3D() {

	CleanUpRenderTarget();
	if (swap_chain) {

		swap_chain->Release();
		swap_chain = NULL;
	}
	if (d3d_device_context) {

		d3d_device_context->Release();
		d3d_device_context = NULL;
	}
	if (d3d_device) {

		d3d_device->Release();
		d3d_device = NULL;
	}
}

void Recipe_Database::WindowWindows::CleanUpRenderTarget() {

	if (main_render_target_view) {

		main_render_target_view->Release();
		main_render_target_view = NULL;
	}
}

bool Recipe_Database::WindowWindows::CreateDeviceD3D(HWND hwnd) {

	// Setup swap chain.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd,
		sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window_handle;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT create_device_flags = 0;
	D3D_FEATURE_LEVEL feature_level;
	const D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_0 };
	if (D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		create_device_flags,
		feature_level_array,
		2,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&d3d_device,
		&feature_level,
		&d3d_device_context) != S_OK) {

		return false;
	}

	CreateRenderTarget();
	return true;
}

void Recipe_Database::WindowWindows::CreateRenderTarget() {

	ID3D11Texture2D* back_buffer;
	swap_chain->GetBuffer(0,
		IID_PPV_ARGS(&back_buffer));
	d3d_device->CreateRenderTargetView(back_buffer,
		NULL,
		&main_render_target_view);
	back_buffer->Release();
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
		ToWstring(window_data->title).c_str(),
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

	// Initialise Direct3D.
	if (!CreateDeviceD3D(window_handle)) {

		CleanUpDeviceD3D();
		UnregisterClass(window_class.lpszClassName,
			window_class.hInstance);
		return;
	}

	ShowWindow(window_handle,
		SW_SHOW);
	UpdateWindow(window_handle);

	// Setup Dear ImGui context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup platform/renderer bindings.
	ImGui_ImplWin32_Init(window_handle);
	ImGui_ImplDX11_Init(d3d_device,
		d3d_device_context);
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
