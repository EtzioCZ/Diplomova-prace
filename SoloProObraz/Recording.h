#pragma once
#include <JuceHeader.h>

class Recording 
{
private:
	std::vector<float> vzorky;

public:
	Recording();
	~Recording();

	void saveSound(double sampleRate, File fullPath);

	AudioBuffer<float> recordingBuffer;


    WavAudioFormat format;
    std::unique_ptr<AudioFormatWriter> writer;
	

};
