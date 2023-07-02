#include "WindowManager.h"

WindowManager& WindowManager::Get()
{
	static WindowManager inst = WindowManager();
	return inst;
}

void WindowManager::PreUpdate()
{
	for (std::unique_ptr<Window>& w : Get()._windows)
	{
		w->PreUpdate();
	}
}
void WindowManager::OnGUI()
{
	for (std::unique_ptr<Window>& w : Get()._windows)
	{
		w->Specs();
		ImGui::Begin(w->name.c_str(), &w->isOpen, w->windowFlags);
		w->OnGUI();
		ImGui::End();
	}
}
void WindowManager::PostUpdate()
{
	for (auto it = Get()._windows.begin(); it != Get()._windows.end();)
	{
		if (!it->get()->isOpen) // remove closed windows
		{
			it->get()->Destroy();
			it = Get()._windows.erase(it);
			continue;
		}
		it->get()->PostUpdate();
		it++;
	}
}
void WindowManager::Destroy()
{
	for (std::unique_ptr<Window>& w : Get()._windows)
	{
		w->Destroy();
	}
}