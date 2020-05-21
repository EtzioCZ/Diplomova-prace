#include "SineGenerator.h"

void SineGenerator::setFrequency(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = tableSize / sampleRate;    
    tableDelta = frequency * tableSizeOverSampleRate;            
}