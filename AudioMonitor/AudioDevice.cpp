#include "AudioDevice.h"

#include <fstream>
#include <string>

namespace
{
    AudioDevice* ad = nullptr;
    // Callback function for audio stream
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData)
    {
        // Cast the input and output buffers to appropriate types
        float* in = (float*)inputBuffer;
		float* out = (float*)outputBuffer;
		for (unsigned int j = 0; j < ad->channelCount; j++)
		{
			for (unsigned int i = 0; i < framesPerBuffer; i++)
            {
				*out++ = *in++;
			}
		}

        return paContinue;
    }
}

void AudioDevice::Shutdown()
{
    // Stop and close the audio stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to stop audio stream: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to close audio stream: " << Pa_GetErrorText(err) << std::endl;
    }

    // Terminate PortAudio
    err = Pa_Terminate();
    if (err != paNoError) {
        std::cerr << "PortAudio termination failed: " << Pa_GetErrorText(err) << std::endl;
    }
    //ad = nullptr;
}

void AudioDevice::LoadLast()
{
    std::ifstream file("settings.properties");
    if (!file.is_open()) return;

    try
    {
        std::string line;
        std::getline(file, line);
        inputDeviceIndex = std::stoi(line);
        std::getline(file, line);
        inputLatency = std::stof(line);
        std::getline(file, line);
        outputDeviceIndex = std::stoi(line);
        std::getline(file, line);
        outputLatency = std::stof(line);
        std::getline(file, line);
        channelCount = std::stoi(line);
    }
    catch (std::exception e) {}

    file.close();
}

void AudioDevice::SaveLast()
{
	try
	{
		std::ofstream file("settings.properties");

		file << inputDeviceIndex << std::endl;
		file << inputLatency << std::endl;
		file << outputDeviceIndex << std::endl;
		file << outputLatency << std::endl;
        file << channelCount << std::endl;

		file.close();
	}
	catch (std::exception e) {}
}

AudioDevice::AudioDevice()
{
    LoadLast();
}

AudioDevice::~AudioDevice()
{
    SaveLast();
    Shutdown();
}

void AudioDevice::Start()
{
    ad = this;
    // Initialize PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
        return;
    }
    // Get the number of available host APIs
    int numHostApis = Pa_GetHostApiCount();
    if (numHostApis < 0) {
        std::cerr << "Failed to get the number of host APIs" << std::endl;
        Pa_Terminate();
        return;
    }

    // Print the available host APIs
    //std::cout << "Available audio APIs:" << std::endl;
    //for (int i = 0; i < numHostApis; i++) {
    //    const PaHostApiInfo* hostApiInfo = Pa_GetHostApiInfo(i);
    //    std::cout << i << ": " << hostApiInfo->name << std::endl;
    //}
    for (int i = 0; i < numHostApis; i++) {
        const PaHostApiInfo* hostApiInfo = Pa_GetHostApiInfo(i);
        driverNames.push_back(hostApiInfo->name);
    }

    // Get the number of available devices for the selected host API
    int numDevices = Pa_GetHostApiInfo(hostApiIndex)->deviceCount;
    if (numDevices < 0) {
        std::cerr << "Failed to get the number of audio devices" << std::endl;
        Pa_Terminate();
        return;
    }

    // Separate input and output devices
    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo->maxInputChannels > 0) {
            inputDeviceIndices.push_back(i);
        }
        if (deviceInfo->maxOutputChannels > 0) {
            outputDeviceIndices.push_back(i);
        }
    }
    //Load Input Names
    inputNames.append("System Default");
    inputNames.push_back('\0');
    for (int i = 1; i < inputDeviceIndices.size(); i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(inputDeviceIndices[i]);
        inputNames.append(deviceInfo->name);
        inputNames.push_back('\0');
    }
    inputNames.pop_back();
    outputNames.append("System Default");
    outputNames.push_back('\0');
    //Load Output Names
    for (int i = 1; i < outputDeviceIndices.size(); i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(outputDeviceIndices[i]);
        outputNames.append(deviceInfo->name);
        outputNames.push_back('\0');
    }
    outputNames.pop_back();

    if (inputDeviceIndex >= inputNames.size()) inputDeviceIndex = 0;
    if (outputDeviceIndex >= outputNames.size()) outputDeviceIndex = 0;

    // Open the audio stream with the selected devices and host API
    inputParameters.device = inputDeviceIndices[inputDeviceIndex];
    inputParameters.channelCount = channelCount;
    inputParameters.sampleFormat = paFloat32;
    //inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.suggestedLatency = inputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    outputParameters.device = outputDeviceIndices[outputDeviceIndex];
    outputParameters.channelCount = channelCount;
    outputParameters.sampleFormat = paFloat32;
    //outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.suggestedLatency = outputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    err = Pa_OpenStream(&stream, &inputParameters, &outputParameters, 44100, bufferSize,
        paNoFlag, audioCallback, nullptr);
    if (err != paNoError) {
        std::cerr << "Failed to open audio stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return;
    }

    // Start the audio stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start audio stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return;
    }
}

void AudioDevice::Restart()
{
    Shutdown();
    Start();
}
