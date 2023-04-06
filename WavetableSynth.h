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
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void updateOsc1Shape(Shape s);
    void updateOsc2Shape(Shape s);
    void setParams(float osc1Gain, float osc1Octave, float osc1Cent, float osc2Gain, float osc2Octave, float osc2Cent);
private:
    static std::vector<float> generateWaveTable(Shape shape);
    static float midiNoteNumberToFrequency(int midiNoteNumber);
    void initializeOscillators();
    void handleMidiEvent(const juce::MidiMessage& midiMessage);
    void render(juce::AudioBuffer<float>& buffer, int beginSample, int endSample);

    double sampleRate;

    std::vector<WavetableOscillator> osc1;
    float osc1Gain;
    float osc1Octave;
    float osc1Cent;
    std::vector<WavetableOscillator> osc2;
    float osc2Gain;
    float osc2Octave;
    float osc2Cent;

    Shape osc1Shape;
};

