#include "WavetableSynth.h"

std::vector<float> WavetableSynth::generateSineWaveTable()
{
    constexpr auto WAVETABLE_LENGTH = 64; //length of wave table
    const auto PI = std::atanf(1.f) * 4; // set PI
    std::vector<float> sineWaveTable = std::vector<float>(WAVETABLE_LENGTH); //Sinus wave table is a vector (of floats) which length is the wavelength

    for (auto i = 0; i < WAVETABLE_LENGTH; ++i) //for all floats in the wavetable (= positions in the wavetable)
    {
        sineWaveTable[i] = 
            std::sinf(                    //sinus of:
                2 * PI * static_cast<float>(i)       //  2 * PI * wavetablePosition 
                /                                    //-----------------------------             
                WAVETABLE_LENGTH);                   //         wavelength        
    }

    return sineWaveTable;
}


//In generateSineWaveTable() we create a vector 
//of a fixed length and fill it
//with samples of one period of the sine.
void WavetableSynth::initializeOscillators()
{
    this->oscillators.clear(); //clear oscillators
    constexpr auto OSCILLATOR_COUNT = 128; //number of oscillators
    const auto sineWaveTable = generateSineWaveTable(); //generate sinus wave table

    for (auto i = 0; i < OSCILLATOR_COUNT; ++i) //For all Oscilators
    {
        oscillators.emplace_back(sineWaveTable, sampleRate); //appends new waveTable to the end of oscillators-vector
    }
}

void WavetableSynth::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate; //set samplerate

    initializeOscillators(); //initialize Oscillators to be ready
}

void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages, double envAmp, double envFreq)
{   
    this->envAmp = envAmp;
    this->envFreq = envFreq;
    this->envInc += envFreq;
    envelope = envAmp * std::sinf(envInc);



    auto currentSample = 0;

    for (const auto midiMetadata : midiMessages) //For all midi messages
    {
        const auto message = midiMetadata.getMessage(); //get the message
        const auto messagePosition = static_cast<int>(message.getTimeStamp()); //get TimeStamp of message

        render(buffer, currentSample, messagePosition); //call function to render block with old and new time stamp 
        currentSample = messagePosition; //set current sample to the timestamp for next message
        handleMidiEvent(message); //handle the mide event according to ts message
    }

    render(buffer, currentSample, buffer.getNumSamples());
}

float WavetableSynth::midiNoteNumberToFrequency(const int midiNoteNumber)
{
    constexpr auto A4_FREQUENCY = 440.f; //A4 reference frequency
    constexpr auto A4_NOTE_NUMBER = 69.f; //A4 reference notenumber
    constexpr auto NOTES_IN_AN_OCTAVE = 12.f; //All Nodes in an Octave
    return A4_FREQUENCY * std::powf(2, (static_cast<float>(midiNoteNumber) - A4_NOTE_NUMBER) / NOTES_IN_AN_OCTAVE); //a4Frequency * (    (noteNumber - a4NoteNumber)/notesInOctave   )^2
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiMessage)
{
    if (midiMessage.isNoteOn()) //if note on
    {
        const auto oscillatorId = midiMessage.getNoteNumber(); //get the number of the node
        const auto frequency = midiNoteNumberToFrequency(oscillatorId); //calculate frequency from noteNumber
        oscillators[oscillatorId].setFrequency(frequency); //set the frequency of the corresponding oscillator to frequency from notenumber
    }
    else if (midiMessage.isNoteOff()) //if note off
    {
        const auto oscillatorId = midiMessage.getNoteNumber(); //get the number of the note
        oscillators[oscillatorId].stop(); //stop corresponding oscillator
    }
    else if (midiMessage.isAllNotesOff()) //if all notes are off
    {
        for (auto& oscillator : oscillators)
        {
            oscillator.stop(); //stop all oscillators
        }
    }
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int beginSample, int endSample)
{
    auto* firstChannel = buffer.getWritePointer(0); //get first outPut Channel
    for (auto& oscillator : oscillators) //for all oscilators
    {
        if (oscillator.isPlaying()) //if the oscillator is playing
        {
            //oscillator.detuneFrequency(envelope);
            for (auto sample = beginSample; sample < endSample; ++sample) // for all samples of the oscillator
            {
                firstChannel[sample] += oscillator.getSample(); //add up all samples to the firstChannel
            }
        }
    }

    //copy the first to all other outPut Channels (TODO: Implement nice effects!)
    for (int channel = 1; channel < buffer.getNumChannels(); ++channel) 
    {
        auto* channelData = buffer.getWritePointer(channel);
        std::copy(firstChannel + beginSample, firstChannel + endSample, channelData + beginSample);
    }
}
