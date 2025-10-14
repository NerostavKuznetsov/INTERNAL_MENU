#include "gui.h"

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../ext/imgui/imgui_impl_dx9.h"

#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);

// Window process 
LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);

bool NK_ImGui::SetupWindowClass(const char* WindowClassName) noexcept
{
	// Populate window class structure	
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowProcess;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandleA(NULL);    
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = WindowClassName;
	WindowClass.hIconSm = NULL;

	// Register class 
	if (!RegisterClassExA(&WindowClass))
		return false;

	return true;
}

void NK_ImGui::DestroyWindowClass() noexcept
{
	UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
}

bool NK_ImGui::SetupWindow(const char* WindowName) noexcept
{
	// Create temp window
	Window = CreateWindow(WindowClass.lpszClassName, WindowName, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, WindowClass.hInstance, 0);
		if (!Window)
			return false;

	return true;
}

void NK_ImGui::DestroyWindow() noexcept
{
	if (Window)
		DestroyWindow(Window);
	
}

bool NK_ImGui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandleA("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));

	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = Window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &params, &device) < 0)
		return false;

	return true;
}

void NK_ImGui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void NK_ImGui::Setup() 
{
	if (!SetupWindowClass("HackClass001"))
		throw std::runtime_error("Failed to setup window class!");
	
	if (!SetupWindow("HackWindow001"))
		throw std::runtime_error("Failed to setup window!");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to setup DirectX!");

	DestroyWindow();
	DestroyWindowClass();
}
 
void NK_ImGui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	Window = params.hFocusWindow;

	OriginalWindowProcess = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess)));

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}
	
void NK_ImGui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Retore window process	
	SetWindowLongPtr(Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(OriginalWindowProcess));

	DestroyDirectX();
}

void NK_ImGui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("NK_ImGui - Nerostav Kuznetsov (ImGui Menu DXD9)", &open);
	ImGui::End();	

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());	
}

LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParam, LPARAM longParam)
{
	// Toggle menu
	if (GetAsyncKeyState(VK_INSERT) & 1)
		NK_ImGui::open = !NK_ImGui::open;

	// Pass messages to imgui
	if (NK_ImGui::open && ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam))
		return 1L;

	return CallWindowProc(NK_ImGui::OriginalWindowProcess, window, message, wideParam, longParam);
}