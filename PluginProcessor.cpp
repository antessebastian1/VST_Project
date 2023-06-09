/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavetableSynthAntesAudioProcessor::WavetableSynthAntesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
#endif
    , waveViewer(1)
    ,parameters(*this, nullptr, juce::Identifier("Parameters"),
        { 
            std::make_unique<juce::AudioParameterFloat>("osc1_gain", "Osc 1 Gain", juce::NormalisableRange{0.0f,1.0f,0.01f, 1.0f, false}, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("osc1_octave", "Osc 1 Octave", juce::NormalisableRange{-3.0f,3.0f,1.0f, 1.0f, false}, 0.0f),
            std::make_unique<juce::AudioParameterFloat>("osc1_cent", "Osc 1 Cent", juce::NormalisableRange{-500.0f,500.0f,1.0f, 1.0f, false}, 0.0f),
            std::make_unique<juce::AudioParameterFloat>("osc2_gain", "Osc 2 Gain", juce::NormalisableRange{0.0f,1.0f,0.01f, 1.0f, false}, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("osc2_octave", "Osc 2 Octave", juce::NormalisableRange{-3.0f,3.0f,1.0f, 1.0f, false}, 0.0f),
            std::make_unique<juce::AudioParameterFloat>("osc2_cent", "Osc 2 Cent", juce::NormalisableRange{-500.0f,500.0f,1.0f, 1.0f, false}, 0.0f),
            std::make_unique<juce::AudioParameterFloat>("attack", "Attack", juce::NormalisableRange{0.1f,5.0f,0.01f, 1.0f, false}, 0.1f),
            std::make_unique<juce::AudioParameterFloat>("decay", "Decay", juce::NormalisableRange{0.1f,2.0f,0.01f, 1.0f, false}, 0.8f),
            std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", juce::NormalisableRange{0.1f,1.0f,0.01f, 1.0f, false}, 0.8f),
            std::make_unique<juce::AudioParameterFloat>("release", "Release", juce::NormalisableRange{0.1f,5.0f,0.01f, 1.0f, false}, 0.8f),
            std::make_unique<juce::AudioParameterFloat>("gain", "Gain", juce::NormalisableRange{0.0f,1.0f,0.01f, 1.0f, false}, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("cutoff_frequency", "Cutoff Frequency", juce::NormalisableRange{20.0f,20000.f,0.1f, 0.2f, false}, 500.f),
            std::make_unique<juce::AudioParameterBool>("highpass", "Highpass",false)
        })
#endif
{   
    osc1GainParam = parameters.getRawParameterValue("osc1_gain");
    osc1OctaveParam = parameters.getRawParameterValue("osc1_octave");
    osc1CentParam = parameters.getRawParameterValue("osc1_cent");
    osc2GainParam = parameters.getRawParameterValue("osc2_gain");
    osc2OctaveParam = parameters.getRawParameterValue("osc2_octave");
    osc2CentParam = parameters.getRawParameterValue("osc2_cent");
    attackParam = parameters.getRawParameterValue("attack");
    decayParam = parameters.getRawParameterValue("decay");
    sustainParam = parameters.getRawParameterValue("sustain");
    releaseParam = parameters.getRawParameterValue("release");
    gainParam = parameters.getRawParameterValue("gain");
    cutoffFreqParam = parameters.getRawParameterValue("cutoff_frequency");
    highpassParam = parameters.getRawParameterValue("highpass");
    waveViewer.setRepaintRate(60);
    waveViewer.setBufferSize(1024);
}

WavetableSynthAntesAudioProcessor::~WavetableSynthAntesAudioProcessor()
{
}

//==============================================================================
const juce::String WavetableSynthAntesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WavetableSynthAntesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WavetableSynthAntesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WavetableSynthAntesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WavetableSynthAntesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WavetableSynthAntesAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WavetableSynthAntesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WavetableSynthAntesAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WavetableSynthAntesAudioProcessor::getProgramName (int index)
{
    return {};
}

void WavetableSynthAntesAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WavetableSynthAntesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.prepareToPlay(sampleRate);
    filter.setSamplingRate(static_cast<float>(sampleRate));
}

void WavetableSynthAntesAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WavetableSynthAntesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const

{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WavetableSynthAntesAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    //clear buffer
    buffer.clear();

    //synthesize Sound
    const auto osc1gain = osc1GainParam->load();
    const auto osc1Octave = osc1OctaveParam->load();
    const auto osc1Cent = osc1CentParam->load();
    const auto osc2gain = osc2GainParam->load();
    const auto osc2Octave = osc2OctaveParam->load();
    const auto osc2Cent = osc2CentParam->load();
    const auto attack = attackParam->load();
    const auto decay = decayParam->load();
    const auto sustain = sustainParam->load();
    const auto release = releaseParam->load();
    synth.setParams(osc1gain, osc1Octave, osc1Cent, osc2gain, osc2Octave, osc2Cent, attack, decay, sustain, release);
    synth.processBlock(buffer, midiMessages);

   

    //apply lowpassHighpassFilter
    //const auto cutoffFreq = cutoffFreqParam->load();20.0f,20000.f
    const auto cutoffFreq = 20.0f + synth.getEnvSample()*19980.0f;
    const auto highpass = *highpassParam < 0.5f ? false : true;
    filter.setCutoffFrequency(cutoffFreq);
    filter.setHighpass(highpass);
    filter.processBlock(buffer, midiMessages);

    //apply Gain
    const auto gain = gainParam->load();
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = buffer.getSample(channel, sample) * gain;
        }
    }

    //Wave visualization:
    waveViewer.pushBuffer(buffer);
}

//==============================================================================
bool WavetableSynthAntesAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WavetableSynthAntesAudioProcessor::createEditor()
{
    return new WavetableSynthAntesAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void WavetableSynthAntesAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WavetableSynthAntesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavetableSynthAntesAudioProcessor();
}

void WavetableSynthAntesAudioProcessor::updateOsc1Shape(Shape osc1Shape) {
    synth.updateOsc1Shape(osc1Shape);
}
void WavetableSynthAntesAudioProcessor::updateOsc2Shape(Shape osc2Shape) {
    synth.updateOsc2Shape(osc2Shape);
}