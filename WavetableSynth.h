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
    void setParams(float osc1Gain, float osc1Octave, float osc2Gain, float osc2Octave);
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
    float osc1Gain;
    float osc1Octave;
    std::vector<WavetableOscillator> osc2;
    float osc2Gain;
    float osc2Octave;

    Shape osc1Shape;
};

