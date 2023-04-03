/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class WavetableSynthAntesAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                 public juce::Slider::Listener

{
public:
    WavetableSynthAntesAudioProcessorEditor (WavetableSynthAntesAudioProcessor&, juce::AudioProcessorValueTreeState& vts);

    ~WavetableSynthAntesAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    juce::GroupComponent gainBox;
    juce::Slider gainSlider;
    juce::Label gainSliderLabel;


    juce::GroupComponent envBox;
    juce::Slider envAmpSlider;
    juce::Label envAmpSliderLabel;
    juce::Slider envFreqSlider;
    juce::Label envFreqSliderLabel;

    juce::GroupComponent filterBox;
    juce::Slider cutoffFreqSlider;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> cutoffFreqAttachment;
    juce::Label cutoffFreqSliderLabel;

    juce::ToggleButton highpassButton;
    std::unique_ptr< juce::AudioProcessorValueTreeState::ButtonAttachment> highpassAttachment;
    juce::Label highpassButtonLabel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WavetableSynthAntesAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynthAntesAudioProcessorEditor)
};
