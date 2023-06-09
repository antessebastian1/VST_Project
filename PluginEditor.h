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
class WavetableSynthAntesAudioProcessorEditor  : public juce::AudioProcessorEditor

{
public:
    WavetableSynthAntesAudioProcessorEditor (WavetableSynthAntesAudioProcessor&, juce::AudioProcessorValueTreeState& vts);

    ~WavetableSynthAntesAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //osc1
    juce::GroupComponent osc1Box;
    //shape
    juce::ComboBox shape1Enum;
    //gain
    juce::Slider osc1GainSlider;
    juce::Label osc1GainSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> osc1GainAttachment;
    //octave
    juce::Slider osc1OctaveSlider;
    juce::Label osc1OctaveSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> osc1OctaveAttachment;
    //cent
    juce::Slider osc1CentSlider;
    juce::Label osc1CentSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> osc1CentAttachment;
    void osc1ShapeChanged();
    
    //osc2
    juce::GroupComponent osc2Box;
    //shape
    juce::ComboBox shape2Enum;
    //gain
    juce::Slider osc2GainSlider;
    juce::Label osc2GainSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> osc2GainAttachment;
    //octave
    juce::Slider osc2OctaveSlider;
    juce::Label osc2OctaveSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> osc2OctaveAttachment;
    //cent
    juce::Slider osc2CentSlider;
    juce::Label osc2CentSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> osc2CentAttachment;
    void osc2ShapeChanged();

    //Envelope ADSR
    juce::GroupComponent envBox;
    //attack
    juce::Slider attackSlider;
    juce::Label attackSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    //decay
    juce::Slider decaySlider;
    juce::Label decaySliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    //sustain
    juce::Slider sustainSlider;
    juce::Label sustainSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    //release
    juce::Slider releaseSlider;
    juce::Label releaseSliderLabel;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;


    //filter on the right side
    juce::GroupComponent filterBox;
    juce::Slider cutoffFreqSlider;
    std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment> cutoffFreqAttachment;
    juce::Label cutoffFreqSliderLabel;
    juce::ToggleButton highpassButton;
    std::unique_ptr< juce::AudioProcessorValueTreeState::ButtonAttachment> highpassAttachment;
    juce::Label highpassButtonLabel;


    //TOTAL gain on the left side
    juce::GroupComponent gainBox;
    juce::Slider gainSlider;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;




    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WavetableSynthAntesAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynthAntesAudioProcessorEditor)
};
