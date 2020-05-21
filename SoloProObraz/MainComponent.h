#pragma once

#include <JuceHeader.h>
#include "CV.h"
#include "SineGenerator.h"
#include <juce_dsp/juce_dsp.h>
#include "Obraz.h"
#include "Analyzer.h"
#include "Utils.h"
#include "Noty.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "Nahled.h"

class MainComponent :   public AudioAppComponent,
                        public FilenameComponentListener,
                        public Timer,
                        public Thread
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================

    // 
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    //==============================================================================

    // vykreslovací metody
    void paint (Graphics& g) override;

    // umístění komponentů v aplikaci i při změně velikosti
    void resized() override;
    //=============================================================================

    // Callbacky jednotlivých komponentů
    void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged) override;
    void playButtonClicked();
    void stopButtonClicked();
    void saveButtonClicked();
    void chromaticButtonClicked();
    void majorButtonClicked();
    void wholetoneButtonClicked();
    void pentatonicButtonClicked();
    void VyskaSliderChanged();
    void VelikostObrazuSliderChanged();
    void DelkaNotSliderChanged();
    void tripletButtonChanged();

    // vytvoření tabulky hodnot
    void createWavetable(int indexObrazu);

    // vytvoření oscilátorů
    void createOscillators(int indexObrazu);

    // periodicky volaná metoda pr o vykreslené analyzéru
    void timerCallback() override;

    // výpočet paramterů obraze v jiném vlákně
    void run() override;

    // ulož zvuk
    void saveSound(double sampleRate, File FileToWrite);


    // instance grafických tříd
    Analyzer m_analyzer;
    Obraz m_obraz;
    Nahled m_nahled;
    Noty m_noty;
    Nota1 m_nota1;
    Nota2 m_nota2;

    // instance a proměnné OpenCV metod
    CV m_cv;
    CV::RGB_pixel rgb_pixel;
    CV::HLS_pixel hls_pixel;
    CV::HSV_pixel hsv_pixel;
    CV::YCbCr_pixel ycbcr_pixel;

    // Oscilátorx 
    OwnedArray<SineGenerator> oscillators;
    AudioSampleBuffer sineTable;

    // velikost okna jako string
    String currentSizeAsString;

    // ADSR obálka
    ADSR m_adsr;
    ADSR::Parameters ADSRpar;

    // wavetable proměnné 
    float currentAngle = 0.0, angleDelta = 0.0, currentSampleRate = 0.0;

    // pomocné proměnné
    double mysampleRate;
    int mybuffer;
    float level = 0.0f;
    int imageIndex = 0;
    float waveIndex = 0.0f;
    int sample_counter = 0;
    int oscillator_index = 0; 
    int recSample = 0;

    // veličiny pro tvorbu zvuku    
    double BPM, delka_noty_ms, delka_noty_vz;
    unsigned char max_pocet_harmonickych = 30;     
    unsigned char min_pocet_harmonickych = 3;
    unsigned char vyska;                            
    int pocet_tonu;
    int midinote_offset = 36;                   //nejnižší možná výška noty

private:
    //==============================================================================
    // komponenty aplikace
    // názvy a jezdce
    std::unique_ptr<Label> levelLabel;
    std::unique_ptr<Slider> levelSlider;  
    std::unique_ptr<Slider> frequencySlider;
    std::unique_ptr<FilenameComponent> fileComp;
    std::unique_ptr<Slider> levelSliderR;
    std::unique_ptr<AudioFormatReaderSource> readerSource; 
    std::unique_ptr<Label> BPMLabel;
    std::unique_ptr<Slider> BPMSlider;
    std::unique_ptr<Label> VyskaLabelTop;
    std::unique_ptr<Label> VyskaLabelBot;
    std::unique_ptr<Slider> VyskaSlider;
    std::unique_ptr<Label> VyskaLabel;
    std::unique_ptr<Label> RootNoteLabel;
    std::unique_ptr<Label> VelikostObrazuLabel;
    std::unique_ptr<Slider> VelikostObrazuSlider;
    std::unique_ptr<Slider> DelkaNotSlider;

    // zaškrtávací pole
    std::unique_ptr<ToggleButton> TripletsButton;
    std::unique_ptr<Label> TripletsLabel;

    // tlačítka
    std::unique_ptr<FileChooser> m_fileChooser;
    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<TextButton> playButton;
    std::unique_ptr<TextButton> stopButton;
    std::unique_ptr<TextButton> chromaticButton;
    std::unique_ptr<TextButton> majorButton;
    std::unique_ptr<TextButton> wholetoneButton;
    std::unique_ptr<TextButton> pentatonicButton;

    AudioProcessor* audioProcesor;

    AudioBuffer<float> recordingBuffer;

    // velikost waveTable
    const unsigned int tableSize = 1 << 11;     
   
    // tabulka názvů tónů
    std::string nazev_tonu[108] = { "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0",\
        "B0", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1", "C2",\
        "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2", "C3", "C#3", "D3",\
        "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3", "C4", "C#4", "D4", "D#4", "E4", \
        "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4", "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5",\
        "G5", "G#5", "A5", "A#5", "B5", "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6",\
        "A6", "A#6", "B6", "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",\
        "C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8", "G#8", "A8", "A#8", "B8" };

    // tabulka posloupnosti DUR stupnice
    int tabulkaMajor[42] = { 0,2,4,5,7,9,11, 12 + 0,12 + 2,12 + 4,12 + 5,12 + 7,12 + 9,12 + 11, 24 + 0,24 + 2,24 + 4,24 + 5,24 + 7,24 + 9,24 + 11,\
        36 + 0,36 + 2,36 + 4,36 + 5,36 + 7,36 + 9,36 + 11,48 + 0, 48 + 2, 48 + 4, 48 + 5, 48 + 7, 48 + 9, 48 + 11, 60 + 0, 60 + 2, 60 + 4, 60 + 5, 60 + 7, 60 + 9, 60 + 11 };

    // tabulka posloupnosti celotónové stupnice
    int tabulkaWholetone[36] = { 0,2,4,6,8,10, 12 + 0,12 + 2,12 + 4,12 + 6, 12 + 8,12 + 10, 24 + 0,24 + 2,24 + 4,24 + 6, 24 + 8,24 + 10, \
        36 + 0,36 + 2,36 + 4,36 + 6, 36 + 8,36 + 10, 48 + 0, 48 + 2, 48 + 4, 48 + 6, 48 + 8, 48 + 10, 60 + 0, 60 + 2, 60 + 4, 60 + 6, 60 + 8, 60 + 10 };

    // tabulka posloupnosti pentatonické stupnice
    int tabulkaPentatonic[30] = { 0,2,4,7,9, 12 + 0,12 + 2,12 + 4,12 + 7,12 + 9, 24 + 0,24 + 2,24 + 4,24 + 7,24 + 9, 36 + 0,36 + 2,36 + 4,36 + 7,36 + 9,\
        48 + 0, 48 + 2, 48 + 4, 48 + 7, 48 + 9, 60 + 0, 60 + 2, 60 + 4, 60 + 7, 60 + 9 };

   // viz matlab - testZvuku.m  - určeno podle rovnoměrného rozložení  p1 = 1.3 :0.05 : 2;    p2 = 2.25 : 0.25 : 3; p3 = 3.5:0.5:4;    p4 = 5 : 1 : 14;
    float tabulkaVahHarm[31] = { 14.f, 13.f, 12.f, 11.f, 10.f,  9.f,  8.f,  7.f,  6.f,  5.f,  4.f,  3.5f, 3.f,  2.75, 2.5f, 2.25f, 2.f, 1.95f, 1.9f, 1.85f, 1.8f, 1.75f, 1.7f, 1.65f, 1.6f, 1.55f,1.5f, 1.45, 1.4f, 1.35f, 1.3f };
    
    // float tabulkaVahHarm[31] = { 1.3f, 1.35f, 1.4f, 1.45f, 1.5f, 1.55f, 1.6f, 1.65f, 1.7f, 1.75f, 1.8f, 1.85f,1.9f, 1.95f, 2.f, 2.25f, 2.5f, 2.75f, 3.f,3.5f,4.f,5.f,6.f,7.f,8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f };  

    // délky not
    float tabulkaDelkyNot[5] = {4.f, 2.f, 1.f, 0.5f, 0.25f };

    //délky not s triolama
    float tabulkaDelkyNotTriplets[8] = { 4.f, 3.f, 2.f, 1.5f, 1.f, 0.75f , 0.5f, 0.25f };

    // poslední stlačené tlačítko
    int buttonClicekdID = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
