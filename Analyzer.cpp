#include "Analyzer.h"

Analyzer::Analyzer() : forwardFFT(fftOrder),      
                       window(fftSize, dsp::WindowingFunction<float>::hann)
                        
{
    setPaintingIsUnclipped(true);
    setAlwaysOnTop(true);
    setSize(getLocalBounds().getWidth(), getLocalBounds().getHeight());
}

void Analyzer::LoadFileErr()
{
    showText = true;
    strDisplayText = "Error loading filename: UTF8 error! \n Full path name must not indlude diacritics!";
    repaint();
}

void Analyzer::LoadFileSuccessfully()
{
    showText = true;
    strDisplayText = "File loaded successfully!!";
    repaint();
}

void Analyzer::FileSavedSuccessfully()
{
    showText = true;
    strDisplayText = "File saved successfully!!";
    repaint();
}

void Analyzer::ShowDefault()
{
    showText = true;
    strDisplayText = "Visualization of sound signal";
    repaint();
}

void Analyzer::paint(Graphics& g)
{
    g.setColour(Colours::white);
    g.fillRect(getLocalBounds());
    g.setColour(findColour(16799488));
    g.fillRect(2, 2, getLocalBounds().getWidth() - 4, getLocalBounds().getHeight() - 4);
    g.setColour(Colours::white);

    if (showText == true)
    {
        g.setColour(Colours::white);
        g.setFont(30.0f);
        g.drawFittedText(strDisplayText, getLocalBounds(), Justification::centred, 2);
    }
    else
    {
        g.setOpacity(1.0f);
        drawFrame(g);
    }
}

void Analyzer::drawFrame(Graphics& g)
{
    for (int i = 1; i < scopeSize; ++i)
    {
        auto width = getLocalBounds().getWidth();    //getLocalBounds
        auto height = getLocalBounds().getHeight();  //get Local Bounds

        g.drawLine({ (float)jmap(i - 1, 0, scopeSize - 1, 0, width),
                            jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                     (float)jmap(i,     0, scopeSize - 1, 0, width),
                            jmap(scopeData[i],     0.0f, 1.0f, (float)height, 0.0f) });
    }
}

void Analyzer::pushNextSampleIntoFifo(float sample) noexcept
{
    if (fifoIndex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }
        fifoIndex = 0;
    }
    fifo[fifoIndex++] = sample;
};

void Analyzer::drawNextFrameOfSpectrum()
{
    window.multiplyWithWindowingTable(fftData, fftSize);     
    forwardFFT.performFrequencyOnlyForwardTransform(fftData);

    auto mindB = -100.0f;
    auto maxdB = 0.0f;

    for (int i = 0; i < scopeSize; ++i)                      
    {
        auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - i / (float)scopeSize) * 0.2f);
        auto fftDataIndex = jlimit(0, fftSize / 2, (int)(skewedProportionX * fftSize / 2));
        auto level = jmap(jlimit(mindB, maxdB, Decibels::gainToDecibels(fftData[fftDataIndex])
            - Decibels::gainToDecibels((float)fftSize)),
            mindB, maxdB, 0.0f, 1.0f);

        scopeData[i] = level;
    }
}