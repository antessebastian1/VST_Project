/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableSynth.h"
#include "LowpassHighpassFilter.h"


//==============================================================================
/**
*/
class WavetableSynthAntesAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    WavetableSynthAntesAudioProcessor();
    ~WavetableSynthAntesAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void updateOsc1Shape(Shape osc1Shape);
    void updateOsc2Shape(Shape osc2Shape);

    juce::AudioVisualiserComponent waveViewer;



private:
    juce::AudioProcessorValueTreeState parameters;
    //synth
    std::atomic<float>* osc1GainParam = nullptr;
    std::atomic<float>* osc1OctaveParam = nullptr;
    std::atomic<float>* osc1CentParam = nullptr;
    std::atomic<float>* osc2GainParam = nullptr;
    std::atomic<float>* osc2OctaveParam = nullptr;
    std::atomic<float>* osc2CentParam = nullptr;
        
    //filter
    std::atomic<float>* cutoffFreqParam = nullptr;
    std::atomic<float>* highpassParam = nullptr;

    //gain
    std::atomic<float>* gainParam = nullptr;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableSynthAntesAudioProcessor)

    WavetableSynth synth;
    LowpassHighpassFilter filter;
};
