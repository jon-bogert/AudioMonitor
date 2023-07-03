#pragma once
#include "AudioDevice.h"
class App
{
public:
	const wchar_t* name = L"Audio Monitor";
	int windowWidth = 400;
	int windowHight = 250;
	bool useFloatingWindows = true;

	AudioDevice* audioDevice = nullptr;
	App() { audioDevice = new AudioDevice; }
	~App() { delete audioDevice; }

	void Start();
	void PreUpdate();
	void OnGUI();
	void PostUpdate();
	void Destroy();
};

