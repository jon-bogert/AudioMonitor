#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <string>
#include <list>
#include <memory>

class Window
{
public:
	bool isOpen = true;
	std::string name = "Window##";
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

	virtual void Start() {}
	virtual void PreUpdate() {}
	virtual void Specs() {}			//To be called before ImGui::Begin()
	virtual void OnGUI() = 0;		//'OnGUI' Must be overridden
	virtual void PostUpdate() {}
	virtual void Destroy() {}
};

class WindowManager final
{
	WindowManager() {}
	static WindowManager& Get();
	std::list<std::unique_ptr<Window>> _windows;

public:
	template <class WinType>
	static WinType* NewWindow();

	static void PreUpdate();
	static void OnGUI();
	static void PostUpdate();
	static void Destroy();
};

template <class WinType>
inline WinType* WindowManager::NewWindow()
{
	static_assert(std::is_base_of_v<Window, WinType>, "WindowManager: Cannot add Non-Window type");
	Get()._windows.push_back(std::make_unique<WinType>());
	Get()._windows.back()->Start();
	return Get()._windows.back().get();
}