#include "WavetableSynth.h"

std::vector<float> WavetableSynth::generateWaveTable(Shape osc1Shape)
{
    constexpr auto WAVETABLE_LENGTH = 64; //length of wave table
    const auto PI = std::atanf(1.f) * 4; // set PI
    std::vector<float> waveTable = std::vector<float>(WAVETABLE_LENGTH); //Sinus wave table is a vector (of floats) which length is the wavelength


    if (osc1Shape == Shape::sinus) {
            for (auto i = 0; i < WAVETABLE_LENGTH; ++i) //for all floats in the wavetable (= positions in the wavetable)
            {
                waveTable[i] =
                    std::sinf(                    //sinus of:
                        2 * PI * static_cast<float>(i)       //  2 * PI * wavetablePosition 
                        /                                    //-----------------------------             
                        WAVETABLE_LENGTH);                   //         wavelength        
            }
    }
    else if (osc1Shape == Shape::saw)
    {
            for (auto i = 0; i < WAVETABLE_LENGTH; ++i) //for all floats in the wavetable (= positions in the wavetable)
            {
                waveTable[i] = -1.0f + 2.0f*static_cast<float>(i)/ WAVETABLE_LENGTH;
            }
    }
    else if (osc1Shape == Shape::square)
    {
        const auto halfLength = (WAVETABLE_LENGTH / 2);
        for (auto i = 0; i < WAVETABLE_LENGTH; ++i) //for all floats in the wavetable (= positions in the wavetable)
        {
            if (i < halfLength) {
                waveTable[i] = 1;
            }
            else {
                waveTable[i] = -1;
            }
        }
    }
    else if (osc1Shape == Shape::triangle)
    {
        const auto halfLength = (WAVETABLE_LENGTH / 2);
        for (auto i = 0; i < WAVETABLE_LENGTH; ++i) //for all floats in the wavetable (= positions in the wavetable)
        {
            waveTable[i] = -1.0f + 4.0f / WAVETABLE_LENGTH * abs(( static_cast<float>(i) - WAVETABLE_LENGTH / 4 % WAVETABLE_LENGTH) - WAVETABLE_LENGTH / 2.0f);
        }
    }
    else if (osc1Shape == Shape::pulse)
    {
         //specifies for which fraction of the period the calue of the waveform is 1
        for (auto i = 0; i < WAVETABLE_LENGTH; ++i) //for all floats in the wavetable (= positions in the wavetable)
        {
            if (i < static_cast<int>(WAVETABLE_LENGTH/8)) {
                waveTable[i] = 1;
            }
            else {
                waveTable[i] = -1;
            }
        }
    }
    

    



    return waveTable;
}


//In generateSineWaveTable() we create a vector 
//of a fixed length and fill it
//with samples of one period of the sine.
void WavetableSynth::initializeOscillators()
{

    this->osc1.clear();
    this->osc2.clear();
    constexpr auto OSCILLATOR_COUNT = 128; //number of oscillators
    auto waveTable = generateWaveTable(Shape::sinus);

    for (auto i = 0; i < OSCILLATOR_COUNT; ++i) //For all Oscilators
    {
        this->osc1.emplace_back(waveTable, sampleRate);
        this->osc2.emplace_back(waveTable, sampleRate);
    }
}

void WavetableSynth::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate; //set samplerate

    initializeOscillators(); //initialize Oscillators to be ready
}

void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{   
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
        osc1[oscillatorId].setFrequency(frequency); //set the frequency of the corresponding oscillator to frequency from notenumber
        osc2[oscillatorId].setFrequency(frequency);
    }
    else if (midiMessage.isNoteOff()) //if note off
    {
        const auto oscillatorId = midiMessage.getNoteNumber(); //get the number of the note
        osc1[oscillatorId].stop(); //stop corresponding oscillator
        osc2[oscillatorId].stop();
    }
    else if (midiMessage.isAllNotesOff()) //if all notes are off
    {
        for (auto& oscillator : osc1)
        {
            oscillator.stop(); //stop all oscillators
        }
        for (auto& oscillator : osc2)
        {
            oscillator.stop(); //stop all oscillators
        }
    }
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int beginSample, int endSample)
{
    auto* firstChannel = buffer.getWritePointer(0); //get first outPut Channel
    for (auto& oscillator : osc1) //for all oscilators
    {
        if (oscillator.isPlaying()) //if the oscillator is playing
        {   
            oscillator.setOctaveAndCent(osc1Octave, osc1Cent);
            for (auto sample = beginSample; sample < endSample; ++sample) // for all samples of the oscillator
            {
                firstChannel[sample] += oscillator.getSample() * osc1Gain; //add up all samples to the firstChannel
            }
        }
    }

    for (auto& oscillator : osc2) //for all oscilators
    {
        if (oscillator.isPlaying()) //if the oscillator is playing
        {   
            oscillator.setOctaveAndCent(osc2Octave, osc2Cent);
            for (auto sample = beginSample; sample < endSample; ++sample) // for all samples of the oscillator
            {
                firstChannel[sample] += oscillator.getSample() * osc2Gain; //add up all samples to the firstChannel
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

void WavetableSynth::updateOsc1Shape(Shape s) {
    osc1.clear();
    auto wavetable = generateWaveTable(s);
    for (auto i = 0; i < 128; ++i) //For all Oscilators
    {
        this->osc1.emplace_back(wavetable, sampleRate);
    }
}
void WavetableSynth::updateOsc2Shape(Shape s) {
    osc2.clear();
    auto wavetable = generateWaveTable(s);
    for (auto i = 0; i < 128; ++i) //For all Oscilators
    {
        this->osc2.emplace_back(wavetable, sampleRate);
    }
}

void WavetableSynth::setParams(float osc1Gain, float osc1Octave, float osc1Cent, float osc2Gain, float osc2Octave, float osc2Cent) {
    this->osc1Gain = osc1Gain;
    this->osc1Octave = osc1Octave;
    this->osc1Cent = osc1Cent;
    this->osc2Gain = osc2Gain;
    this->osc2Octave = osc2Octave;
    this->osc2Cent = osc2Cent;
}
