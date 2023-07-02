#include "App.h"
#include "imgui.h"
namespace
{
	
}

void App::Start()
{
	audioDevice->Start();
}

void App::PreUpdate()
{
}

void App::OnGUI()
{
	
	ImGui::Combo("Input Device", &audioDevice->inputDeviceIndex, audioDevice->inputNames.c_str());
	ImGui::DragFloat("Input Latency", &audioDevice->inputLatency, 0.001f, 0.f, 1.f);

	ImGui::NewLine();

	ImGui::Combo("Output Device", &audioDevice->outputDeviceIndex, audioDevice->outputNames.c_str());
	ImGui::DragFloat("Output Latency", &audioDevice->outputLatency, 0.001f, 0.f, 1.f);

	ImGui::NewLine();

	if (ImGui::Button("Apply")) audioDevice->Restart();
}

void App::PostUpdate()
{
}

void App::Destroy()
{
}
