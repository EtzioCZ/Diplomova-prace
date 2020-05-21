#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include "Utils.h"

class Analyzer : public Component
{

public:
    Analyzer();
    //vykreslovací metoda 
    void paint(Graphics& g) override;             
    // vykreslovací metoda FFT
    void drawFrame(Graphics& g);          
    // plnìní FIFO
    void pushNextSampleIntoFifo(float sample) noexcept;      
    // výpoèet bodù pro analyzér
    void drawNextFrameOfSpectrum();                            

    // soubor hlášek pro uživatele

    // Obraz neúspìšnì nahrán
    void LoadFileErr();
    // Obraz úspìšnì nahrán
    void LoadFileSuccessfully();
    // Obraz úspìšnì uložen
    void FileSavedSuccessfully();
    // Ukaž default
    void ShowDefault();

    String strDisplayText = "Visualization of sound signal";

    // nastavení vlastností FFT
    enum
    {
        fftOrder = 12,
        fftSize = 1 << fftOrder,
        scopeSize = 2048
    };

    dsp::FFT forwardFFT;
    dsp::WindowingFunction<float> window;

    bool showText = true;
    float fifo[fftSize] = { 0 };
    float fftData[2 * fftSize] = { 0 };
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float scopeData[scopeSize] = { 0 };

};
