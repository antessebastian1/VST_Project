#pragma once
#include <JuceHeader.h>

#include "WavetableOscillator.h"

enum Shape
{
    sinus,
    triangle,
    square,
    saw,
    pulse
};

class WavetableSynth
{
public:
    void prepareToPlay(double sampleRate);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, double envAmp, double envFreq);
    void updateOsc1Shape(Shape s);
    void updateOsc2Shape(Shape s);

private:
    static std::vector<float> generateWaveTable(Shape shape);
    static float midiNoteNumberToFrequency(int midiNoteNumber);
    void initializeOscillators();
    void handleMidiEvent(const juce::MidiMessage& midiMessage);
    void render(juce::AudioBuffer<float>& buffer, int beginSample, int endSample);

    double sampleRate;
    //std::vector<WavetableOscillator> sinOscillators;
    //std::vector<WavetableOscillator> sawOscillators;
    //std::vector<WavetableOscillator> squareOscillators;

    std::vector<WavetableOscillator> osc1;
    std::vector<WavetableOscillator> osc2;

    double envAmp;
    double envFreq;
    double envelope = 0.0f;
    double envInc = 0.0f;
    Shape osc1Shape;
};

