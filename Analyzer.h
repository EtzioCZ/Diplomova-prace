#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>
#include "Utils.h"

class Analyzer : public Component
{

public:
    Analyzer();
    //vykreslovac� metoda 
    void paint(Graphics& g) override;             
    // vykreslovac� metoda FFT
    void drawFrame(Graphics& g);          
    // pln�n� FIFO
    void pushNextSampleIntoFifo(float sample) noexcept;      
    // v�po�et bod� pro analyz�r
    void drawNextFrameOfSpectrum();                            

    // soubor hl�ek pro u�ivatele

    // Obraz ne�sp�n� nahr�n
    void LoadFileErr();
    // Obraz �sp�n� nahr�n
    void LoadFileSuccessfully();
    // Obraz �sp�n� ulo�en
    void FileSavedSuccessfully();
    // Uka� default
    void ShowDefault();

    String strDisplayText = "Visualization of sound signal";

    // nastaven� vlastnost� FFT
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