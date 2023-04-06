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


    //osc1
    addAndMakeVisible(osc1Box);
    osc1Box.setText("OSC 1");
    osc1Box.setColour(osc1Box.outlineColourId, juce::Colours::black.withAlpha(0.2f));
    osc1Box.setColour(osc1Box.textColourId, juce::Colours::black);

    //shape
    addAndMakeVisible(shape1Enum);
    shape1Enum.addItem("Sinus",1);
    shape1Enum.addItem("Saw",2);
    shape1Enum.addItem("Square",3);
    shape1Enum.onChange = [this] { osc1ShapeChanged(); };
    shape1Enum.setSelectedId(1);

    //gain
    addAndMakeVisible(osc1GainSliderLabel);
    osc1GainSliderLabel.setText("Gain", juce::dontSendNotification);
    osc1GainSliderLabel.setColour(osc1GainSliderLabel.textColourId, juce::Colours::black);

    addAndMakeVisible(osc1GainSlider);
    osc1GainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    osc1GainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        vts, "osc1_gain", osc1GainSlider));
    osc1GainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    osc1GainSlider.setColour(osc1GainSlider.textBoxTextColourId, juce::Colours::black);
    osc1GainSlider.setColour(osc1GainSlider.thumbColourId, juce::Colours::black); //Set Knob colour
    osc1GainSlider.setColour(osc1GainSlider.trackColourId, juce::Colours::black.withAlpha(0.2f)); //set slider track colour

    //octave
    addAndMakeVisible(osc1OctaveSliderLabel);
    osc1OctaveSliderLabel.setText("Octave", juce::dontSendNotification);
    osc1OctaveSliderLabel.setColour(osc1OctaveSliderLabel.textColourId, juce::Colours::black);

    addAndMakeVisible(osc1OctaveSlider);
    osc1OctaveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    osc1OctaveAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        vts, "osc1_octave", osc1OctaveSlider));
    osc1OctaveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    osc1OctaveSlider.setColour(osc1OctaveSlider.textBoxTextColourId, juce::Colours::black);
    osc1OctaveSlider.setColour(osc1OctaveSlider.thumbColourId, juce::Colours::black); //Set Knob colour
    osc1OctaveSlider.setColour(osc1OctaveSlider.trackColourId, juce::Colours::black.withAlpha(0.2f)); //set slider track colour


    //osc2
    addAndMakeVisible(osc2Box);
    osc2Box.setText("OSC 2");
    osc2Box.setColour(osc2Box.outlineColourId, juce::Colours::black.withAlpha(0.2f));
    osc2Box.setColour(osc2Box.textColourId, juce::Colours::black);

    addAndMakeVisible(shape2Enum);
    shape2Enum.addItem("Sinus", 1);
    shape2Enum.addItem("Saw", 2);
    shape2Enum.addItem("Square", 3);
    shape2Enum.onChange = [this] { osc2ShapeChanged(); };
    shape2Enum.setSelectedId(1);

    addAndMakeVisible(osc2GainSliderLabel);
    osc2GainSliderLabel.setText("Gain", juce::dontSendNotification);
    osc2GainSliderLabel.setColour(osc2GainSliderLabel.textColourId, juce::Colours::black);

    addAndMakeVisible(osc2GainSlider);
    osc2GainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    osc2GainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        vts, "osc2_gain", osc2GainSlider));
    osc2GainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    osc2GainSlider.setColour(osc2GainSlider.textBoxTextColourId, juce::Colours::black);
    osc2GainSlider.setColour(osc2GainSlider.thumbColourId, juce::Colours::black); //Set Knob colour
    osc2GainSlider.setColour(osc2GainSlider.trackColourId, juce::Colours::black.withAlpha(0.2f)); //set slider track colour

    //octave
    addAndMakeVisible(osc2OctaveSliderLabel);
    osc2OctaveSliderLabel.setText("Octave", juce::dontSendNotification);
    osc2OctaveSliderLabel.setColour(osc2OctaveSliderLabel.textColourId, juce::Colours::black);

    addAndMakeVisible(osc2OctaveSlider);
    osc2OctaveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    osc2OctaveAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        vts, "osc2_octave", osc2OctaveSlider));
    osc2OctaveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    osc2OctaveSlider.setColour(osc2OctaveSlider.textBoxTextColourId, juce::Colours::black);
    osc2OctaveSlider.setColour(osc2OctaveSlider.thumbColourId, juce::Colours::black); //Set Knob colour
    osc2OctaveSlider.setColour(osc2OctaveSlider.trackColourId, juce::Colours::black.withAlpha(0.2f)); //set slider track colour



    //Filter
    filterBox.setText("Filter");
    filterBox.setColour(filterBox.outlineColourId, juce::Colours::black.withAlpha(0.2f));
    filterBox.setColour(filterBox.textColourId, juce::Colours::black);
    addAndMakeVisible(filterBox);


    addAndMakeVisible(cutoffFreqSlider);
    cutoffFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        vts, "cutoff_frequency", cutoffFreqSlider));
    cutoffFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    cutoffFreqSlider.setColour(cutoffFreqSlider.textBoxTextColourId, juce::Colours::black);
    cutoffFreqSlider.setColour(cutoffFreqSlider.thumbColourId, juce::Colours::black); //Set Knob colour
    cutoffFreqSlider.setColour(cutoffFreqSlider.trackColourId, juce::Colours::black.withAlpha(0.2f)); //set slider track colour


    addAndMakeVisible(cutoffFreqSliderLabel);
    cutoffFreqSliderLabel.setText("Freq", juce::dontSendNotification);
    cutoffFreqSliderLabel.setColour(cutoffFreqSliderLabel.textColourId, juce::Colours::black);

    addAndMakeVisible(highpassButton);
    highpassButton.setColour(highpassButton.tickColourId, juce::Colours::black);
    highpassButton.setColour(highpassButton.tickDisabledColourId, juce::Colours::black);

    highpassAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        vts, "highpass", highpassButton));

    addAndMakeVisible(highpassButtonLabel);
    highpassButtonLabel.setText("Highpass", juce::dontSendNotification);
    highpassButtonLabel.setColour(highpassButtonLabel.textColourId, juce::Colours::black);


    //Gainbox
    gainBox.setText("Master");
    gainBox.setColour(gainBox.outlineColourId, juce::Colours::black.withAlpha(0.2f));
    gainBox.setColour(gainBox.textColourId, juce::Colours::black);
    addAndMakeVisible(gainBox);

    //gain slider
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setColour(gainSlider.textBoxTextColourId, juce::Colours::black);
    gainSlider.setValue(-1.0);
    gainSlider.setColour(gainSlider.thumbColourId, juce::Colours::black); //Set Knob colour
    gainSlider.setColour(gainSlider.trackColourId, juce::Colours::black.withAlpha(0.2f)); //set slider track colour
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        vts, "gain", gainSlider));
    addAndMakeVisible(gainSlider);



    //wave Viewer
    audioProcessor.waveViewer.setColours(juce::Colours::black, juce::Colours::limegreen.withAlpha(0.5f));
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
    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId,juce::Colours::lightgrey);
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    
}

void WavetableSynthAntesAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto waveViewerHeight = 0.5f;



    auto leftMargin = getHeight() * 0.06125;
    auto topMargin = getHeight() * 0.06125;


    auto groupWidth = getWidth() * 0.06125;
    auto groupHeight = getHeight() - waveViewerHeight * getHeight() - topMargin * 2;
    auto sliderWidth = 0.8 * groupWidth;
    auto sliderHeight = 0.8 * groupHeight;
   
    auto bottomMargin = getHeight() - waveViewerHeight - topMargin;

    auto filterBoxXPos = getWidth() - leftMargin - 3 * groupWidth;


    

 
    //osc1 Box
    osc1Box.setBounds(leftMargin, topMargin, 2 * groupWidth, groupHeight);
    //shapeEnum
    shape1Enum.setBounds(leftMargin + 0.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderWidth/4);
    //gain slider
    osc1GainSlider.setBounds(leftMargin + 1.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderWidth);
    osc1GainSliderLabel.setBounds(leftMargin + 1.1 * groupWidth + 0.5f * sliderWidth - 20, topMargin + 0.1 * groupHeight - 15, sliderWidth, 20);
    //octave slider
    osc1OctaveSlider.setBounds(leftMargin + 1.1 * groupWidth, topMargin + 1.2* sliderWidth + 0.1 * groupHeight, sliderWidth, sliderWidth);
    osc1OctaveSliderLabel.setBounds(leftMargin + 1.1 * groupWidth + 0.5f * sliderWidth - 30, topMargin + 1.2 * sliderWidth + 0.1 * groupHeight - 15, sliderWidth, 20);

    //osc2 Box
    osc2Box.setBounds(leftMargin + 2* groupWidth, topMargin, 2 * groupWidth, groupHeight);
    //shapeEnum
    shape2Enum.setBounds(leftMargin + 2.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderWidth/4);
    //gain slider
    osc2GainSlider.setBounds(leftMargin + 3.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderWidth);
    osc2GainSliderLabel.setBounds(leftMargin + 3.1 * groupWidth + 0.5f * sliderWidth - 20, topMargin + 0.1 * groupHeight - 15, sliderWidth, 20);
    //octave slider
    osc2OctaveSlider.setBounds(leftMargin + 3.1 * groupWidth, topMargin + 1.2 * sliderWidth + 0.1 * groupHeight, sliderWidth, sliderWidth);
    osc2OctaveSliderLabel.setBounds(leftMargin + 3.1 * groupWidth + 0.5f * sliderWidth - 30, topMargin + 1.2 * sliderWidth + 0.1 * groupHeight - 15, sliderWidth, 20);


    //LopassHighpassFilter
    filterBox.setBounds(filterBoxXPos, topMargin, groupWidth * 2, groupHeight);
    //cutoff slider
    cutoffFreqSlider.setBounds(filterBoxXPos + 0.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderHeight);
    cutoffFreqSliderLabel.setBounds(filterBoxXPos + 0.1 * groupWidth + 0.5f * sliderWidth - 20, topMargin + 0.1 * groupHeight - 15, sliderWidth, 20);
    //highpass button
    highpassButton.setBounds(filterBoxXPos + 1.1 * groupWidth + 0.5f * sliderWidth, topMargin + 0.1 * groupHeight, sliderWidth, 0.1 * groupHeight);
    highpassButtonLabel.setBounds(filterBoxXPos + 1.1 * groupWidth + 0.5f * sliderWidth - 20, topMargin + 0.1 * groupHeight - 15, sliderWidth, 20);


    //gain box
    gainBox.setBounds(getWidth() - leftMargin - groupWidth, topMargin, groupWidth, groupHeight);
    //gain slider
    gainSlider.setBounds(getWidth() - leftMargin - groupWidth + 0.1 * groupWidth, topMargin + 0.1 * groupHeight, sliderWidth, sliderWidth);


    //wave visualizer // on bottom center
    audioProcessor.waveViewer.setBounds( 0.0, waveViewerHeight * getHeight(), getWidth(), getHeight() * waveViewerHeight);
}   


void WavetableSynthAntesAudioProcessorEditor::osc1ShapeChanged()
{
    switch (shape1Enum.getSelectedId())
    {
    case 1: audioProcessor.updateOsc1Shape(Shape::sinus);
        break;
    case 2: audioProcessor.updateOsc1Shape(Shape::saw);
        break;
    case 3: audioProcessor.updateOsc1Shape(Shape::square);
        break;
    }
}

void WavetableSynthAntesAudioProcessorEditor::osc2ShapeChanged()
{
    switch (shape2Enum.getSelectedId())
    {
    case 1: audioProcessor.updateOsc2Shape(Shape::sinus);
        break;
    case 2: audioProcessor.updateOsc2Shape(Shape::saw);
        break;
    case 3: audioProcessor.updateOsc2Shape(Shape::square);
        break;
    }
}
