#include "App.h"
#include "imgui.h"
namespace
{
	const char* channelOptions = "Mono\0Stereo";
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
	
	if (ImGui::Combo("Input Device", &audioDevice->inputDeviceIndex, audioDevice->inputNames.c_str()))
	{
		audioDevice->Restart();
	}
	if (ImGui::DragFloat("Input Latency", &audioDevice->inputLatency, 0.001f, 0.f, 1.f))
	{
		audioDevice->Restart();
	}

	ImGui::NewLine();

	if (ImGui::Combo("Output Device", &audioDevice->outputDeviceIndex, audioDevice->outputNames.c_str()))
	{
		audioDevice->Restart();
	}
	if (ImGui::DragFloat("Output Latency", &audioDevice->outputLatency, 0.001f, 0.f, 1.f))
	{
		audioDevice->Restart();
	}

	ImGui::NewLine();

	int cCount = audioDevice->channelCount - 1;
	if (ImGui::Combo("Output Mode", &cCount, channelOptions))
	{
		audioDevice->channelCount = cCount + 1;
		audioDevice->Restart();
	}
}

void App::PostUpdate()
{
}

void App::Destroy()
{
}
