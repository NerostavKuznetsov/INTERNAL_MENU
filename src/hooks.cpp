#include "hooks.h"

#include <stdexcept>
#include <intrin.h>

#include "../ext/minhook/minhook.h"

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../ext/imgui/imgui_impl_dx9.h"

void NK_Hooks::Setup()
{
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize MinHook!");

	if (MH_CreateHook(VirtualFunction(NK_ImGui::device, 42), &EndScene, reinterpret_cast<void**>(&EndSceneOriginal)))
		throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(VirtualFunction(NK_ImGui::device, 16), &Reset, reinterpret_cast<void**>(&ResetOriginal)))
		throw std::runtime_error("Unable to hook Reset()");
		
	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable MinHook!"); 

	NK_ImGui::DestroyDirectX();
}

void NK_Hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall NK_Hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	// stop endscene getting called twice
	if (_ReturnAddress() == returnAddress)
		return result;

	if (!NK_ImGui::setup)
		NK_ImGui::SetupMenu(device);

	if (NK_ImGui::open)
		NK_ImGui::Render();

	return result;
}

HRESULT __stdcall NK_Hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}