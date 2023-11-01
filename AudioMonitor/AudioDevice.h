#pragma once
#include <iostream>
#include <vector>

#include "portaudio.h"

class AudioDevice
{
	PaError err;

	void LoadLast();
	void SaveLast();

public:
	std::vector<std::string> driverNames;
	std::string inputNames;
	std::string outputNames;

	std::vector<int> inputDeviceIndices;
	std::vector<int> outputDeviceIndices;

	int hostApiIndex = 0;
	int inputDeviceIndex = 0;
	int outputDeviceIndex = 0;

	int channelCount = 1;
	float inputLatency = 0.07f;
	float outputLatency = 0.07f;
	int bufferSize = 2048;

	PaStream* stream;
	PaStreamParameters inputParameters;
	PaStreamParameters outputParameters;

	AudioDevice();
	~AudioDevice();

	void Start();
	void Restart();
	void Shutdown();

	void SetVolFactor(const float newGain);
	float GetVolFactor() const;
	bool GetIsClipping() const;

};

