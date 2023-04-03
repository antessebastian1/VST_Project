#include <cmath>
#include "JuceHeader.h"
#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator(std::vector<float> waveTable, double sampleRate)
    : waveTable{ std::move(waveTable) },
    sampleRate{ sampleRate }
{}

float WavetableOscillator::getSample()
{
    // To enforce the invariant that only an active oscillator will 
    // have its getSample() member function called, I have added an assertion that it isPlaying().
    jassert(isPlaying());
    // The next step is to bring the index into the range of wave table indices using std::fmod. 
    // std::fmod returns the floating-point remainder of a division and, thus, keeps the index
    // within the [0, waveTable.size()) range.
    index = std::fmod(index, static_cast<float>(waveTable.size()));
    // Afterwards, we perform a linear interpolation of wave table values to get the output sample.
    const auto sample = interpolateLinearly();
    // Only then do we increment the index; otherwise, we would never start playing a note with 
    // index equal to 0 (we would always start looping at indexIncrement).
    index += indexIncrement;
    return sample;
}

void WavetableOscillator::setFrequency(float frequency)
{
    //indexincrement is the frequency times the size of the wavetable(=Wavelength) divided by the sample Rate    i = (f*w)/s
    indexIncrement = frequency * static_cast<float>(waveTable.size())
        / static_cast<float>(sampleRate);
}

void WavetableOscillator::detuneFrequency(float deltaFreq)
{
    indexIncrement = indexIncrement + deltaFreq * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
}

void WavetableOscillator::stop()
{
    //clear index and index increment
    index = 0.f;
    indexIncrement = 0.f;
}

bool WavetableOscillator::isPlaying() const
{
    //if the oscillator is playing, the indexincrement cant be 0, so return true if it is not 0 and return false if it is 0.
    return indexIncrement != 0.f;
}

float WavetableOscillator::interpolateLinearly() const
{
    const auto truncatedIndex = static_cast<typename  decltype(waveTable)::size_type>(index); //truncate due to taylor polynom (waveTableSize)
    const auto nextIndex = static_cast<typename  decltype(waveTable)::size_type> //next index is index%waveTable.size
        (std::ceil(index)) % waveTable.size();
    const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
    return waveTable[nextIndex] * nextIndexWeight +
        (1.f - nextIndexWeight) * waveTable[truncatedIndex];
}