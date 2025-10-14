#pragma once 
#include <d3d9.h>

namespace NK_ImGui // ( Nerostav Kuznetsov )
{
	// Show menu? 
	inline bool open = true;

	// is menu setup?
	inline bool setup = false;

	// WINAPI related
	inline HWND Window = nullptr;
	inline WNDCLASSEX WindowClass = { };
	inline WNDPROC OriginalWindowProcess = nullptr;

	// DX stuff
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool SetupWindowClass(const char* WindowClassName) noexcept; 
	void DestroyWindowClass() noexcept;  

	bool SetupWindow(const char* WindowName) noexcept; 		
	void DestroyWindow() noexcept;

	bool SetupDirectX() noexcept;
	void DestroyDirectX() noexcept;

	// Setup device 
	void Setup();

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void Destroy() noexcept;

	void Render() noexcept;
}