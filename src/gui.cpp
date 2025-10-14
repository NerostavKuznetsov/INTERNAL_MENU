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

}

void NK_ImGui::DestroyWindowClass() noexcept
{

}

bool NK_ImGui::SetupWindow(const char* WindowName) noexcept
{

}

void NK_ImGui::DestroyWindow() noexcept
{

}

bool NK_ImGui::SetupDirectX() noexcept
{

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