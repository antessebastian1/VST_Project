#pragma once

#include <vector>
#include "JuceHeader.h"

class LowpassHighpassFilter {

public:
	void setHighpass(bool highpass);
	void setCutoffFrequency(float cutoffFreq);
	void setSamplingRate(float samplingRate);
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

private:
	bool highpass;
	float cutoffFreq;
	float samplingRate;
	std::vector<float> dnBuffer;

};
