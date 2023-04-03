/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynthAntesAudioProcessorEditor::WavetableSynthAntesAudioProcessorEditor (WavetableSynthAntesAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1920, 1080);
    setTitle("Operierer");


    //boundary border for Gain
    gainBox.setText("Gain");
    gainBox.setColour(gainBox.outlineColourId, juce::Colours::red.withAlpha(0.2f));
    gainBox.setColour(gainBox.textColourId, juce::Colours::red);
    addAndMakeVisible(gainBox);

    //gain slider
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setColour(gainSlider.textBoxTextColourId, juce::Colours::black);
    gainSlider.setRange(-48.0, 0.0);
    gainSlider.setValue(-1.0);
    gainSlider.addListener(this);
    gainSlider.setColour(gainSlider.thumbColourId, juce::Colours::red); //Set Knob colour
    gainSlider.setColour(gainSlider.trackColourId, juce::Colours::red.withAlpha(0.2f)); //set slider track colour
    addAndMakeVisible(gainSlider);

    gainSliderLabel.setText("Gain", juce::dontSendNotification);
    gainSliderLabel.setColour(gainSliderLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(gainSliderLabel);

    //boundary border for Envelope
    envBox.setText("LFO 1");
    envBox.setColour(envBox.outlineColourId, juce::Colours::red.withAlpha(0.2f));
    envBox.setColour(envBox.textColourId, juce::Colours::red);
    addAndMakeVisible(envBox);

    //envelope Amplitude slider
    envAmpSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    envAmpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    envAmpSlider.setColour(envAmpSlider.textBoxTextColourId, juce::Colours::black);
    envAmpSlider.setRange(0.0, 200.0);
    envAmpSlider.setValue(0.0);
    envAmpSlider.addListener(this);
    envAmpSlider.setColour(envAmpSlider.thumbColourId, juce::Colours::red); //Set Knob colour
    envAmpSlider.setColour(envAmpSlider.trackColourId, juce::Colours::red.withAlpha(0.2f)); //set slider track colour
    addAndMakeVisible(envAmpSlider);

    envAmpSliderLabel.setText("Amp", juce::dontSendNotification);
    envAmpSliderLabel.setColour(envAmpSliderLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(envAmpSliderLabel);

    //envelope Frequency slider
    envFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    envFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    envFreqSlider.setColour(envFreqSlider.textBoxTextColourId, juce::Colours::black);
    envFreqSlider.setRange(0.0, 1.0);
    envFreqSlider.setValue(0.0);
    envFreqSlider.addListener(this);
    envFreqSlider.setColour(envFreqSlider.thumbColourId, juce::Colours::red); //Set Knob colour
    envFreqSlider.setColour(envFreqSlider.trackColourId, juce::Colours::red.withAlpha(0.2f)); //set slider track colour
    addAndMakeVisible(envFreqSlider);

    envFreqSliderLabel.setText("Freq", juce::dontSendNotification);
    envFreqSliderLabel.setColour(envFreqSliderLabel.textColourId, juce::Colours::black);
    addAndMakeVisible(envFreqSliderLabel);


    //lowpassHighpass filter
    //boundary border for Envelope
    filterBox.setText("Filter");
    filterBox.setColour(filterBox.outlineColourId, juce::Colours::red.withAlpha(0.2f));
    filterBox.setColour(filterBox.textColourId, juce::Colours::red);
    addAndMakeVisible(filterBox);


    addAndMakeVisible(cutoffFreqSlider);
    cutoffFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        vts, "cutoff_frequency", cutoffFreqSlider));
    cutoffFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    cutoffFreqSlider.setColour(cutoffFreqSlider.textBoxTextColourId, juce::Colours::black);
    cutoffFreqSlider.setColour(cutoffFreqSlider.thumbColourId, juce::Colours::red); //Set Knob colour
    cutoffFreqSlider.setColour(cutoffFreqSlider.trackColourId, juce::Colours::red.withAlpha(0.2f)); //set slider track colour


    addAndMakeVisible(cutoffFreqSliderLabel);
    cutoffFreqSliderLabel.setText("Freq", juce::dontSendNotification);
    cutoffFreqSliderLabel.setColour(cutoffFreqSliderLabel.textColourId, juce::Colours::black);

    addAndMakeVisible(highpassButton);
    highpassButton.setColour(highpassButton.tickColourId, juce::Colours::red);
    highpassButton.setColour(highpassButton.tickDisabledColourId, juce::Colours::black);

    highpassAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        vts, "highpass", highpassButton));

    addAndMakeVisible(highpassButtonLabel);
    highpassButtonLabel.setText("Highpass", juce::dontSendNotification);
    highpassButtonLabel.setColour(highpassButtonLabel.textColourId, juce::Colours::black);








    //wave Viewer
    audioProcessor.waveViewer.setColours(juce::Colours::black, juce::Colours::red.withAlpha(0.5f));
    audioProcessor.waveViewer.setTitle("Wave Visualizer");
    addAndMakeVisible(audioProcessor.waveViewer);


}

WavetableSynthAntesAudioProcessorEditor::~WavetableSynthAntesAudioProcessorEditor()
{
}

//==============================================================================
void WavetableSynthAntesAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId,juce::Colours::beige);
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    
}

void WavetableSynthAntesAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto leftMargin = getHeight() * 0.06125;
    auto topMargin = getHeight() * 0.06125;
    auto groupWidth = getWidth() * 0.06125;
    auto groupHeight = getHeight() * 0.5f - topMargin*2;
    auto sliderWidth = 0.8 * groupWidth;
    auto sliderHeight = 0.8 * groupHeight;
    
    auto waveViewerHeight = 0.5f;
    auto bottomMargin = getHeight() - waveViewerHeight - topMargin;


    //gain box
    gainBox.setBounds(leftMargin, topMargin, groupWidth, groupHeight);
    //gain slider
    gainSlider.setBounds(leftMargin + 0.1*groupWidth, topMargin + 0.1*groupHeight, sliderWidth, sliderHeight);
    //gain label
    gainSliderLabel.setBounds(leftMargin + 0.1 * groupWidth, topMargin + 0.1 * groupHeight - 20, sliderWidth, 20);

    //env box
    envBox.setBounds(leftMargin + 2 * groupWidth, topMargin, groupWidth*2, groupHeight);
    //env Slider
    envAmpSlider.setBounds((leftMargin + 2 * groupWidth) + 0.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderHeight);
    envAmpSliderLabel.setBounds((leftMargin + 2 * groupWidth) + 0.1 * groupWidth + 0.5f * sliderWidth - 15, topMargin + 0.1 * groupHeight - 20, sliderWidth, 20);

    envFreqSlider.setBounds((leftMargin + 2 * groupWidth) + 1.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderHeight);
    envFreqSliderLabel.setBounds((leftMargin + 2 * groupWidth) + 1.1 * groupWidth + 0.5f * sliderWidth - 20, topMargin + 0.1 * groupHeight - 20, sliderWidth, 20);
    //LopassHighpassFilter
    filterBox.setBounds(leftMargin + 5 * groupWidth, topMargin, groupWidth * 2, groupHeight);
    //cutoff slider
    cutoffFreqSlider.setBounds((leftMargin + 5 * groupWidth) + 0.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderHeight);
    cutoffFreqSliderLabel.setBounds((leftMargin + 5 * groupWidth) + 0.1 * groupWidth + 0.5f * sliderWidth - 20, topMargin + 0.1 * groupHeight - 20, sliderWidth, 20);
    //highpass button
    highpassButton.setBounds((leftMargin + 5 * groupWidth) + 1.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderHeight);
    highpassButtonLabel.setBounds((leftMargin + 5 * groupWidth) + 1.1 * groupWidth - 40, topMargin + 0.1 * groupHeight - 20 + 0.5*sliderHeight, sliderWidth, 20);




    //wave visualizer // on bottom center
    audioProcessor.waveViewer.setBounds( 0.0, waveViewerHeight * getHeight(), getWidth(), getHeight() * waveViewerHeight);
}   


void WavetableSynthAntesAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        audioProcessor.rawVolume = pow(10, gainSlider.getValue() / 20);
    }

    if (slider == &envAmpSlider)
    {
        audioProcessor.envAmp = envAmpSlider.getValue();
    }

    if (slider == &envFreqSlider)
    {
        audioProcessor.envFreq = envFreqSlider.getValue();
    }
}
