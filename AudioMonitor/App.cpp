#include "App.h"
#include "imgui.h"
namespace
{
	const char* channelOptions = "Mono\0Stereo";
	const char* bufferOptions = " 16\0 32\0 64\0 128\0 256\0 512\0 1024\0 2048\0 4086\0 8172";
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

	int bSize = 7;
	switch (audioDevice->bufferSize)
	{
	case 16:
		bSize = 0;
		break;
	case 32:
		bSize = 1;
		break;
	case 64:
		bSize = 2;
		break;
	case 128:
		bSize = 3;
		break;
	case 256:
		bSize = 4;
		break;
	case 512:
		bSize = 5;
		break;
	case 1024:
		bSize = 6;
		break;
	case 2048:
		bSize = 7;
		break;
	case 4096:
		bSize = 8;
		break;
	case 8172:
		bSize = 9;
		break;
	}
	
	if (ImGui::Combo("Buffer Size", &bSize, bufferOptions))
	{
		switch (bSize)
		{
		case 0:
			audioDevice->bufferSize = 16;
			break;
		case 1:
			audioDevice->bufferSize = 32;
			break;
		case 2:
			audioDevice->bufferSize = 64;
			break;
		case 3:
			audioDevice->bufferSize = 128;
			break;
		case 4:
			audioDevice->bufferSize = 256;
			break;
		case 5:
			audioDevice->bufferSize = 512;
			break;
		case 6:
			audioDevice->bufferSize = 1024;
			break;
		case 7:
			audioDevice->bufferSize = 2048;
			break;
		case 8:
			audioDevice->bufferSize = 4096;
			break;
		case 9:
			audioDevice->bufferSize = 8172;
			break;
		}
		audioDevice->Restart();
	}
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
