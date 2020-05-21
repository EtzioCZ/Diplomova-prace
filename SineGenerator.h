#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class SineGenerator
{
public:
    SineGenerator (const AudioSampleBuffer& wavetableToUse, int indexObrazu)
        : wavetable (wavetableToUse),
          tableSize (wavetable.getNumSamples() -1 ),
          indexObrazu(indexObrazu)
    {
    }
    // výpoèet hodnoty tabulky pro zvolenou frekvenci
    void setFrequency(float frequency, float sampleRate);      

    // vrátí další prvek z tabulky 
    forcedinline float getNextSample() noexcept { 

        auto index0 = (unsigned int)currentIndex;
        auto index1 = index0 + 1;

        auto frac = currentIndex - (float)index0;

        auto* table = wavetable.getReadPointer(indexObrazu);
        auto value0 = table[index0];
        auto value1 = table[index1];

        // výpoèet interpolace
        float currentSample = value0 + frac * (value1 - value0); 
      
        if ((currentIndex += tableDelta) > tableSize)
            currentIndex -= tableSize;
  
        return currentSample;
    }

    // pomocné promìnné
    float currentIndex = 0.0f, tableDelta = 0.0f;
private:

    const AudioSampleBuffer& wavetable;
    const int tableSize;
    int indexObrazu;
};



