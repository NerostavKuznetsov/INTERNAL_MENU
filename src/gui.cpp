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
	WindowClass.hInstance = GetModuleHandleA(nullptr);
	WindowClass.hIcon = nullptr;
	WindowClass.hCursor = nullptr;
	WindowClass.hbrBackground = nullptr;
	WindowClass.lpszMenuName = nullptr;
	WindowClass.lpszClassName = WindowClassName;
	WindowClass.hIconSm = nullptr;

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
}

void NK_ImGui::DestroyDirectX() noexcept
{

}

void NK_ImGui::Setup() noexcept
{

}

void NK_ImGui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{

}

void NK_ImGui::Destroy() noexcept
{

}

void NK_ImGui::Render() noexcept
{

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