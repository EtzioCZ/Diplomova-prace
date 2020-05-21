#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : Thread("SegmentaceObrazu")

{
    audioProcesor = new AudioProcessorGraph();

    fileComp.reset(new FilenameComponent("Select image to play from...",
        {},                       // current file
        false,                    // can edit file name,
        false,                    // is directory,
        false,                    // is for saving,
        { "*.jpg;*.jpeg;*.png" },                       // browser wildcard suffix,
        {},                       // enforced suffix,
        "Select your image to play from..."));  // text when nothing selected
    fileComp->setBrowseButtonText("Load File");
    fileComp->setDefaultBrowseTarget(File::getSpecialLocation(File::userPicturesDirectory));
    fileComp->setMaxNumberOfRecentFiles(5);
    addAndMakeVisible(fileComp.get());
    fileComp->addListener(this);

    levelSlider.reset(new Slider());
    levelSliderR.reset(new Slider());
    frequencySlider.reset(new Slider());
    levelLabel.reset(new Label());
    BPMSlider.reset(new Slider());
    BPMLabel.reset(new Label());
    VyskaLabelTop.reset(new Label());
    VyskaLabelBot.reset(new Label());
    VyskaSlider.reset(new Slider());
    VelikostObrazuLabel.reset(new Label());
    VelikostObrazuSlider.reset(new Slider());
    VyskaLabel.reset(new Label()); 
    RootNoteLabel.reset(new Label());
    DelkaNotSlider.reset(new Slider());
    TripletsButton.reset(new ToggleButton());
    TripletsLabel.reset(new Label());
    playButton.reset(new TextButton());
    stopButton.reset(new TextButton());
    saveButton.reset(new TextButton());
    chromaticButton.reset(new TextButton());
    majorButton.reset(new TextButton());
    wholetoneButton.reset(new TextButton());
    pentatonicButton.reset(new TextButton());

    // level Label
    levelLabel->setText("Volume", dontSendNotification);
    addAndMakeVisible(levelLabel.get());

    // Level Slider
    levelSlider->setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    levelSlider->setColour(Slider::backgroundColourId, Colours::white);
    levelSlider->setTextBoxIsEditable(false);
    levelSlider->setColour(Slider::trackColourId, Colours::lightslategrey);
    levelSlider->setColour(Slider::textBoxTextColourId, Colours::white);
    levelSlider->setRange(0.0, 0.25);
    levelSlider->setValue(0.20);
    levelSlider->setSkewFactorFromMidPoint(0.08);
    levelSlider->setNumDecimalPlacesToDisplay(3);
    addAndMakeVisible(levelSlider.get());

   // Play Button
    playButton->setButtonText("Play");
    playButton->setColour(TextButton::buttonColourId, Colours::green);
    playButton->setClickingTogglesState(true);
    playButton->onClick = [this] { playButtonClicked(); };
    playButton->setEnabled(false);
    addAndMakeVisible(playButton.get());

    // Stop Button
    stopButton->setButtonText("Stop");
    stopButton->onClick = [this] { stopButtonClicked(); };
    stopButton->setColour(TextButton::buttonColourId, Colours::red);
    stopButton->setEnabled(false);
    addAndMakeVisible(stopButton.get());

    // Save Button
    saveButton->onClick = [this] { saveButtonClicked(); };
    saveButton->setButtonText("Save file");
    saveButton->setColour(TextButton::buttonColourId, Colours::lightgrey);
    saveButton->setEnabled(false);
    addAndMakeVisible(saveButton.get());

    // ostatní komponenty
    addAndMakeVisible(m_obraz);
    addAndMakeVisible(m_nahled);
    addAndMakeVisible(m_analyzer);
    
    // BPM
    BPMSlider->setRange(45, 250, 1);
    BPMSlider->setValue(120);
    BPMSlider->setTextBoxIsEditable(true);
    BPMSlider->setTextBoxStyle(Slider::TextBoxAbove, false,getWidth(),getHeight());
    BPMSlider->setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    BPMSlider->setSliderSnapsToMousePosition(false);
    BPMSlider->setColour(Slider::trackColourId, Colours::transparentBlack);
    BPMSlider->setSize(60, 20);
    addAndMakeVisible(BPMSlider.get());

    // Délka not label
    BPMLabel->setText("BPM", dontSendNotification);
    addAndMakeVisible(BPMLabel.get());
    
    // Délka not slider
    DelkaNotSlider->setSliderStyle(Slider::SliderStyle::TwoValueVertical);
    DelkaNotSlider->setTextBoxStyle(Slider::TextBoxLeft, false, getWidth(), getHeight());
    DelkaNotSlider->setColour(Slider::backgroundColourId, Colours::white);
    DelkaNotSlider->setColour(Slider::trackColourId, Colours::lightslategrey);
    DelkaNotSlider->setColour(Slider::textBoxTextColourId, Colours::white);
    DelkaNotSlider->setRange(0, 4, 1);
    DelkaNotSlider->setMinAndMaxValues(1, 3, dontSendNotification);
    DelkaNotSlider->onValueChange = [this] {DelkaNotSliderChanged(); };
    addAndMakeVisible(DelkaNotSlider.get());

    // Noty
    addAndMakeVisible(m_nota1);
    m_nota1.choosenImage1 = m_noty.pulova;
    addAndMakeVisible(m_nota2);
    m_nota2.choosenImage2 = m_noty.osminova;

    // Triplets
    addAndMakeVisible(TripletsButton.get());
    TripletsButton->onClick = [this] {tripletButtonChanged(); };
    TripletsLabel->setText("Triplets", dontSendNotification);
    addAndMakeVisible(TripletsLabel.get());

    // Vyska Slider
    VyskaSlider->setSliderStyle(Slider::SliderStyle::TwoValueVertical);
    VyskaSlider->setTextBoxStyle(Slider::TextBoxLeft, false, getWidth(), getHeight());
    VyskaSlider->setColour(Slider::backgroundColourId, Colours::white);
    VyskaSlider->setColour(Slider::trackColourId, Colours::lightslategrey);
    VyskaSlider->setColour(Slider::textBoxTextColourId, Colours::white);
    VyskaSlider->setRange(0, 71, 1);
    VyskaSlider->setMinAndMaxValues(12, 36, dontSendNotification);
    VyskaSlider->onValueChange = [this] {VyskaSliderChanged(); };
    addAndMakeVisible(VyskaSlider.get());

    // Vyska label
    VyskaLabelTop->setText(nazev_tonu[36 + midinote_offset], dontSendNotification);
    VyskaLabelBot->setText(nazev_tonu[12 + midinote_offset], dontSendNotification);
    addAndMakeVisible(VyskaLabelBot.get());
    addAndMakeVisible(VyskaLabelTop.get());

    // Root note label
    RootNoteLabel->setText("Root note", dontSendNotification);
    addAndMakeVisible(RootNoteLabel.get());

    // Velikost obrazu slider
    VelikostObrazuSlider->setRange(5, 100, 1);
    VelikostObrazuSlider->setValue(25);
    VelikostObrazuSlider->setTextBoxIsEditable(true);
    VelikostObrazuSlider->setTextBoxStyle(Slider::TextBoxAbove, false, 80 , 30);
    VelikostObrazuSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    VelikostObrazuSlider->setSliderSnapsToMousePosition(false);
    VelikostObrazuSlider->setColour(Slider::trackColourId, Colours::transparentBlack);   
    VelikostObrazuSlider->onValueChange = [this] {VelikostObrazuSliderChanged(); };
    addAndMakeVisible(VelikostObrazuSlider.get());

    // Velikost obrazu label
    VelikostObrazuLabel->setText("Number of sounds [n*n]", dontSendNotification);
    addAndMakeVisible(VelikostObrazuLabel.get());

    // Chromatic Button
    chromaticButton->setButtonText("Chromatic");
    chromaticButton->setColour(TextButton::textColourOffId, Colours::black);
    chromaticButton->onClick = [this] {chromaticButtonClicked(); };
    chromaticButton->setColour(TextButton::buttonColourId, Colours::white);
    chromaticButton->setEnabled(false);
    addAndMakeVisible(chromaticButton.get());

    // Major Button
    majorButton->setButtonText("Major");
    majorButton->onClick = [this] {majorButtonClicked(); };
    majorButton->setColour(TextButton::buttonColourId, Colours::saddlebrown);
    majorButton->setEnabled(true);
    addAndMakeVisible(majorButton.get());

    // Wholetone Button
    wholetoneButton->setButtonText("Wholetone");
    wholetoneButton->onClick = [this] {wholetoneButtonClicked(); };
    wholetoneButton->setColour(TextButton::buttonColourId, Colours::mediumslateblue);
    wholetoneButton->setEnabled(true);
    addAndMakeVisible(wholetoneButton.get());

    // pentatonic Button
    pentatonicButton->setButtonText("Pentatonic");
    pentatonicButton->onClick = [this] {pentatonicButtonClicked(); };
    pentatonicButton->setColour(TextButton::buttonColourId, Colours::orange);
    pentatonicButton->setEnabled(true);
    addAndMakeVisible(pentatonicButton.get());

    setSize(1280, 700);

    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (0, 2); });
    }
    else
    {
        setAudioChannels (0, 2);
    }

}
MainComponent::~MainComponent()
{
    shutdownAudio();
    delete audioProcesor;
    remove("resized.png");
}
void MainComponent::releaseResources()
{
}
//==============================================================================

//==============================================================================
void MainComponent::paint (Graphics& g) 
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.fillAll(Colours::transparentBlack);
   // Image background = ImageCache::getFromMemory(BinaryData::Start_png, BinaryData::Start_pngSize);
   // g.drawImage(background, 0, 0, getWidth(), getHeight(), 0, 0, background.getWidth(), background.getHeight());            //backgroud image

    g.setColour(Colours::black);
    g.setFont(20.0f);
    g.drawText(currentSizeAsString, getLocalBounds(), Justification::bottomRight, true);

    g.setFont(16.0f);
    g.setColour(Colours::white);
    g.drawMultiLineText("Tone \n range", getWidth() * 0.87, -50 + getHeight() / 2, 12 / 2, Justification::centred);

    g.drawMultiLineText("Note \n length", getWidth() * 0.92, -56 + getHeight() / 2, 12 / 2, Justification::centred);

}
void MainComponent::resized()
{
    auto xVyska =  0.84;
    auto xDelka = 0.89;
    auto xTlacita = 0.71;
    auto xNota = 0.02;
    auto yNota = 0.05;

        levelLabel->setBounds(getWidth() - 74, 20, 60, 40);                        //lavel popisek
        levelSlider->setBounds(getWidth() - 70, 60, 50, getHeight()-100);          // level slider vpravo
        currentSizeAsString = String(getWidth()) + " x " + String(getHeight());     // size vpravo dole 

        fileComp->setBounds(50, getHeight() - 100, getWidth() * 0.5 -80, 60);       //dialog box
        m_obraz.setBounds(50, 150, getWidth()*0.65, getHeight()- 270);              //obraz
        m_nahled.setBounds(getWidth() * xTlacita, 20,104, 104);                     // nahled
        m_analyzer.setBounds(50, 10, getWidth() * 0.65, 120);                       // FFT
                  
        VelikostObrazuLabel->setBounds(getWidth() * (0.8), 20, 80, 40);
        VelikostObrazuSlider->setBounds(getWidth() * (0.805), 60, 60, 60);

        //talčítka
        playButton->setBounds(getWidth() * 0.5 , getHeight() - 100, getWidth()* 0.125 , 60);                                                             // playbutton
        stopButton->setBounds(getWidth() * 0.5 + playButton->getWidth() + 30, getHeight() - 100, getWidth() * 0.125, 60);                                // stop button 
        saveButton->setBounds(getWidth() * 0.5 + playButton->getWidth() + 60 + stopButton->getWidth(), getHeight() - 100, getWidth() * 0.125, 60);       // save button
        chromaticButton->setBounds(getWidth() * xTlacita, 120+getHeight()* 0.09, getWidth() * 0.1, getHeight() * 0.09);
        majorButton->setBounds(getWidth() * xTlacita, 140 + getHeight() * (0.18), getWidth() * (0.1), getHeight() * (0.09));
        wholetoneButton->setBounds(getWidth() * xTlacita, 160 + getHeight() * (0.27), getWidth() * (0.1), getHeight() * (0.09));
        pentatonicButton->setBounds(getWidth() * xTlacita, 180 + getHeight() * (0.36), getWidth() * (0.1), getHeight() * (0.09));

        // delka not
        BPMLabel->setBounds(getWidth() * (xDelka) + 10, 20, 60, 40);
        BPMSlider->setBounds(getWidth() * (xDelka), 60, 60, 32);
        TripletsLabel->setBounds(getWidth() * (xDelka+0.01), 100, 60, 30);
        TripletsButton->setBounds(getWidth() * (xDelka-0.01), 100, 30, 30);
        DelkaNotSlider->setBounds(getWidth() * (xDelka), 130, 50, getHeight() * 0.8 - 120);
        m_nota1.setBounds(getWidth() * (xDelka + 0.032), 140, getWidth()*xNota, getHeight()*yNota);
        m_nota2.setBounds(getWidth() * (xDelka + 0.032), getHeight() * 0.75 , getWidth() * xNota, getHeight() * yNota);

        // vyska not
   //   VyskaLabel->setBounds(getWidth() * (xVyska-0.1), 140 + VyskaSlider->getWidth() / 2, 60, 60);
        VyskaSlider->setBounds(getWidth() * (xVyska), 130, 50, getHeight() * 0.8 - 120);
        VyskaLabelTop->setBounds(getWidth() * (xVyska - 0.02), 140, 35, 30);
        VyskaLabelBot->setBounds(getWidth() * (xVyska - 0.02), 100 + VyskaSlider->getHeight() , 35, 30);
        RootNoteLabel->setBounds(getWidth() * (xVyska - 0.025), 70 + VyskaSlider->getHeight(), 40, 40);

}
void MainComponent::createWavetable(int indexObrazu)
{
    float* samples = sineTable.getWritePointer(indexObrazu);

    // pocet harmonickych
    std::vector<int> harmonics;
    for (int numOfHarm = 0; numOfHarm < m_cv.HSV_q[indexObrazu].S; numOfHarm++)
    {
        harmonics.push_back(numOfHarm + 1);
    }

    // váhy jednotlivých harmonických
    std::vector<float> harmonicWeights;
    for (float i = 0; i < m_cv.HSV_q[indexObrazu].S ; i++)
    {
            float ind = i;
            harmonicWeights.push_back( powf(tabulkaVahHarm[m_cv.HLS_q[indexObrazu].L], -ind) + 0.003f);      
    }

    jassert(harmonics.size() == harmonicWeights.size());

    //generování sinů do tabulky
    for (size_t harmonic = 0; harmonic < harmonics.size(); ++harmonic)
    {
        angleDelta = MathConstants<float>::twoPi / (float)(tableSize - 1) * harmonics[harmonic];

        currentAngle = 0.0;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            float sample = std::sin(currentAngle);
            samples[i] += sample * harmonicWeights[harmonic];
            currentAngle += angleDelta;
        }
    }

    samples[tableSize] = samples[0];
}
void MainComponent::createOscillators(int indexObrazu)
{
    float numberOfOscillators = 1;
    int midiNote;

    for (auto i = 0; i < numberOfOscillators; ++i)
    {
        auto* oscillator = new SineGenerator(sineTable, indexObrazu);
        if (!majorButton->isEnabled())
        {
            midiNote = midinote_offset/*36*/ + tabulkaMajor[m_cv.YCbCr_q[indexObrazu].Y] + double2int(VyskaSlider->getMinValue());
        }
        else if (!wholetoneButton->isEnabled())
        {
            midiNote = midinote_offset/*36*/ + tabulkaWholetone[m_cv.YCbCr_q[indexObrazu].Y] + double2int(VyskaSlider->getMinValue());
        }
        else if (!pentatonicButton->isEnabled())
        {
            midiNote = midinote_offset/*36*/ + tabulkaPentatonic[m_cv.YCbCr_q[indexObrazu].Y] + double2int(VyskaSlider->getMinValue());
        }
        else
        {
            midiNote = midinote_offset/*36*/ + m_cv.YCbCr_q[indexObrazu].Y + double2int(VyskaSlider->getMinValue());
        }

        float frequency = 440.0 * pow(2.0, (midiNote - 69.0) / 12.0);

        oscillator->setFrequency(frequency, double2float(deviceManager.getAudioDeviceSetup().sampleRate));
        oscillators.add(oscillator);
    }
    level = 0.25f / numberOfOscillators;
}
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    mysampleRate = audioProcesor->getSampleRate();
    mysampleRate = sampleRate;
    mybuffer = samplesPerBlockExpected;
    m_adsr.setSampleRate(mysampleRate); 

    ADSRpar.attack = 0.005f;        // 5 ms
    ADSRpar.decay = 0.002f;        // 2 ms
    ADSRpar.sustain = 0.9f;        // 0.9 level
    ADSRpar.release = 0.005f;       // 5 ms
    m_adsr.setParameters(ADSRpar);

}
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{

    if (!playButton->getToggleState() || !stopButton->isEnabled())
    {
        bufferToFill.clearActiveBufferRegion();
    }
    else
    {
        
        float* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        float* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
        bufferToFill.clearActiveBufferRegion();

        recordingBuffer.setSize(1, mybuffer+recSample, true, true, false);
        float* recordBuffer = recordingBuffer.getWritePointer(0);

        level = double2float(levelSlider->getValue());
        BPM = BPMSlider->getValue();


        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {         
            
            auto* oscillator = oscillators.getUnchecked(oscillator_index);

            if (oscillator->currentIndex == 0)
                oscillator->currentIndex = waveIndex;
          
            if (!m_adsr.isActive())
                m_adsr.noteOn();

            float adsr = m_adsr.getNextSample();

            float levelSample = oscillator->getNextSample()  * adsr;
            float recordingSample = levelSample * 0.25f;
            levelSample = levelSample * level;

            if (recordingSample > 1.f)
                recordingSample = 1.f;
            if (recordingSample < -1.f)
                recordingSample = -1.f;

            if (levelSample > 1.f)
                levelSample = 1.f;  
            if (levelSample < -1.f)
                levelSample = -1.f;

            leftBuffer[sample] = levelSample;
            rightBuffer[sample] = levelSample;

            recordBuffer[recSample] = recordingSample;

            m_analyzer.pushNextSampleIntoFifo(levelSample);
            ++sample_counter;
            ++recSample;

            if (TripletsButton->getToggleState() == true)
            {
                delka_noty_ms = (60000.000 / (BPM * tabulkaDelkyNotTriplets[m_cv.HSV_q[oscillator_index].V]));
            }
            else
            {
                delka_noty_ms = (60000.000 / (BPM * tabulkaDelkyNot[m_cv.HSV_q[oscillator_index].V]));
            }            
            delka_noty_vz = mysampleRate * (delka_noty_ms * 0.001);
          
            if (sample_counter == floor(delka_noty_vz - ADSRpar.release*mysampleRate))
                m_adsr.noteOff();

            if (sample_counter == floor(delka_noty_vz))
            {
                sample_counter = 0;
                ++oscillator_index;
                waveIndex = oscillator->currentIndex;
                m_adsr.noteOn();

                if (oscillator_index == oscillators.size())
                {
                    stopButton->triggerClick();
                    bufferToFill.clearActiveBufferRegion();
                    sample = bufferToFill.numSamples;
                    oscillator_index = 0;
                    sample_counter = 0;
                    recSample = 0;
                    MessageManagerLock mml;
                    m_analyzer.repaint();
                }
            }
        }
    }
}
void MainComponent::saveSound(double sampleRate, File FileToWrite)
{
    FileToWrite.getFullPathName();
    FileToWrite.deleteFile();


    unique_ptr<WavAudioFormat> wavFormat = make_unique<WavAudioFormat>();
    unique_ptr<AudioFormatWriter> writer;

    writer.reset(wavFormat->createWriterFor(new FileOutputStream(FileToWrite),
        sampleRate,
        1,
        24,
        {},
        0));

    if (writer != nullptr)
    {
        writer->writeFromAudioSampleBuffer(recordingBuffer, 0, recordingBuffer.getNumSamples());
        recordingBuffer.clear();
    }
}
//==============================================================================
void MainComponent::filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged)    // dialog text clicked
{
    if (fileComponentThatHasChanged == fileComp.get())
    {
       // fileComp->setEnabled(false);
        
        String filename(fileComp->getCurrentFileText());
       
        if (!CharPointer_ASCII::isValidString(filename.getCharPointer().getAddress(), filename.length()))
        {
            StringArray straTmp = fileComp->getRecentlyUsedFilenames();
            straTmp.removeString(filename);
            fileComp->setRecentlyUsedFilenames(straTmp);
            m_analyzer.LoadFileErr();
            return;
        }

        m_cv.clearAllExceptOriginal();
        m_cv.m_mat_original.release(); 
        m_cv.imageRead(filename.toStdString());
        m_cv.clearAllExceptOriginal();
        m_cv.imageResizeOriginal(double2int(VelikostObrazuSlider->getValue()), double2int(VelikostObrazuSlider->getValue()));
        startThread();

        m_analyzer.LoadFileSuccessfully();
        m_obraz.loadedImage = ImageFileFormat::loadFrom(fileComp->getCurrentFile());
        m_obraz.repaint();

       // m_cv.imageResize(100, 100);
        File resized(File::getCurrentWorkingDirectory().getChildFile("resized.png"));
        m_nahled.nahledImage = ImageFileFormat::loadFrom(resized);
        m_nahled.repaint();
    }
}
void MainComponent::playButtonClicked()
{
    BPMSlider->setEnabled(false);
    fileComp->setEnabled(false);
    VyskaSlider->setEnabled(false);
    VyskaLabelTop->setEnabled(false);
    VyskaLabelBot->setEnabled(false);
    VelikostObrazuSlider->setEnabled(false);
    TripletsButton->setEnabled(false);
    DelkaNotSlider->setEnabled(false);

    chromaticButton->setEnabled(false);
    majorButton->setEnabled(false);
    wholetoneButton->setEnabled(false);
    pentatonicButton->setEnabled(false);

    
    if (playButton->getToggleState() == true)
    {
        playButton->setButtonText("Pause");
    }
    else
    {
        playButton->setButtonText("Play");
        playButton->setColour(TextButton::buttonColourId, Colours::green);
    }
    stopButton->setEnabled(true);
    saveButton->setEnabled(false);

    m_analyzer.setOpaque(true);
    startTimerHz(30);
}
void MainComponent::stopButtonClicked()
{
    oscillator_index = 0;
    sample_counter = 0;
    recSample = 0;
    stopButton->setEnabled(false);
    playButton->setToggleState(false, NotificationType::dontSendNotification);
    playButton->setButtonText("Play");
    playButton->setColour(TextButton::buttonColourId, Colours::green);
    BPMSlider->setEnabled(true);
    fileComp->setEnabled(true);
    VyskaSlider->setEnabled(true);
    VyskaLabelTop->setEnabled(true);
    VyskaLabelBot->setEnabled(true);
    VelikostObrazuSlider->setEnabled(true);
    TripletsButton->setEnabled(true);
    DelkaNotSlider->setEnabled(true);
    saveButton->setEnabled(true);

    switch (buttonClicekdID)
    {
    case 0:
        chromaticButton->setEnabled(false);
        majorButton->setEnabled(true);
        wholetoneButton->setEnabled(true);
        pentatonicButton->setEnabled(true);
        break;        
    case 1:
        chromaticButton->setEnabled(true);
        majorButton->setEnabled(false);
        wholetoneButton->setEnabled(true);
        pentatonicButton->setEnabled(true);
        break;
    case 2:
        chromaticButton->setEnabled(true);
        majorButton->setEnabled(true);
        wholetoneButton->setEnabled(false);
        pentatonicButton->setEnabled(true);
        break;
    case 3:
        chromaticButton->setEnabled(true);
        majorButton->setEnabled(true);
        wholetoneButton->setEnabled(true);
        pentatonicButton->setEnabled(false);
        break;
    }
    stopTimer();
    m_analyzer.ShowDefault();
}
void MainComponent::saveButtonClicked()
{
    if (recordingBuffer.getNumSamples() > 0)
    {
        m_fileChooser.reset(new FileChooser("Select a location to save",
            File::getSpecialLocation(File::userMusicDirectory),
            "*.wav"));
        if (m_fileChooser->browseForFileToSave(true))
        {
            File savedFileName(m_fileChooser->getResult());
            saveSound(mysampleRate, savedFileName);
            m_analyzer.FileSavedSuccessfully();
            saveButton->setEnabled(false);                
        }
    }
}
void MainComponent::chromaticButtonClicked()
{
    if (!stopButton->isEnabled())
    {
        buttonClicekdID = 0;
        chromaticButton->setEnabled(false);
        majorButton->setEnabled(true);
        wholetoneButton->setEnabled(true);
        pentatonicButton->setEnabled(true);
        VyskaSlider->setRange(0, 71, 1);
        VyskaSliderChanged();
    }
}
void MainComponent::majorButtonClicked()
{
    if (!stopButton->isEnabled())
    {
        buttonClicekdID = 1;
        chromaticButton->setEnabled(true);
        majorButton->setEnabled(false);
        wholetoneButton->setEnabled(true);
        pentatonicButton->setEnabled(true);
        VyskaSlider->setRange(0, 41, 1);
        VyskaSliderChanged();
    }
}
void MainComponent::wholetoneButtonClicked()
{
    if (!stopButton->isEnabled())
    {
        buttonClicekdID = 2;
        chromaticButton->setEnabled(true);
        majorButton->setEnabled(true);
        wholetoneButton->setEnabled(false);
        pentatonicButton->setEnabled(true);
        VyskaSlider->setRange(0, 35, 1);
        VyskaSliderChanged();
    }
}
void MainComponent::pentatonicButtonClicked()
{
    if (!stopButton->isEnabled())
    {
        buttonClicekdID = 3;
        chromaticButton->setEnabled(true);
        majorButton->setEnabled(true);
        wholetoneButton->setEnabled(true);
        pentatonicButton->setEnabled(false);
        VyskaSlider->setRange(0, 29, 1);       
        VyskaSliderChanged();
    }
}
void MainComponent::VyskaSliderChanged()
{
    int i = double2int(VyskaSlider->getMaxValue());
    int j = double2int(VyskaSlider->getMinValue());
    int tabulkaI;
   
    if (!majorButton->isEnabled())
    {
        tabulkaI = j + tabulkaMajor[i - j];
    }
    else if (!wholetoneButton->isEnabled())
    {
        tabulkaI = j + tabulkaWholetone[i - j];
    }
    else if (!pentatonicButton->isEnabled())
    {
        tabulkaI = j + tabulkaPentatonic[i - j];
    }
    else
    {
        tabulkaI = i;
    }
        VyskaLabelTop->setText(nazev_tonu[tabulkaI + midinote_offset], dontSendNotification);
        VyskaLabelBot->setText(nazev_tonu[j + midinote_offset], dontSendNotification);

        if (!m_cv.m_mat.empty())
        {
            oscillators.clear();
   
            for (int index = 0; index < m_cv.m_mat.rows * m_cv.m_mat.cols; index++)
            {
                m_cv.quantizeUchar(m_cv.YCbCr[index].Y, m_cv.YCbCr_q[index].Y, 0, 255, 0, (float)(i - j + 1));                   // vyska tonu - pocet tonu
                createOscillators(index);
            }
        }
        VyskaSlider->repaint();
}
void MainComponent::DelkaNotSliderChanged()
{
    int i = double2int(DelkaNotSlider->getMaxValue());
    int j = double2int(DelkaNotSlider->getMinValue());

    if (!m_cv.m_mat.empty())
    {     
        for (int index = 0; index < m_cv.m_mat.rows * m_cv.m_mat.cols; index++)
        {
            m_cv.quantizeUchar(m_cv.HSV[index].V, m_cv.HSV_q[index].V, 0, 255, int2float(j), int2float(i));                   // Delka tonu 
        }
    }

    if (TripletsButton->getToggleState()== true)
    {
        switch (i)
        {
        case 0:
            m_nota1.choosenImage1 = m_noty.sestnactinova;
            break;
        case 1:
            m_nota1.choosenImage1 = m_noty.osminovaTriola;
            break;
        case 2:
            m_nota1.choosenImage1 = m_noty.osminova;
            break;
        case 3:
            m_nota1.choosenImage1 = m_noty.ctvrtovaTriola;
            break;
        case 4:
            m_nota1.choosenImage1 = m_noty.ctvrtova;
            break;
        case 5:
            m_nota1.choosenImage1 = m_noty.pulovaTriola;
            break;
        case 6:
            m_nota1.choosenImage1 = m_noty.pulova;
            break;
        case 7:
            m_nota1.choosenImage1 = m_noty.cela;
            break;
        }
        switch (j)
        {
        case 0:
            m_nota2.choosenImage2 = m_noty.sestnactinova;
            break;
        case 1:
            m_nota2.choosenImage2 = m_noty.osminovaTriola;
            break;
        case 2:
            m_nota2.choosenImage2 = m_noty.osminova;
            break;
        case 3:
            m_nota2.choosenImage2 = m_noty.ctvrtovaTriola;
            break;
        case 4:
            m_nota2.choosenImage2 = m_noty.ctvrtova;
            break;
        case 5:
            m_nota2.choosenImage2 = m_noty.pulovaTriola;
            break;
        case 6:
            m_nota2.choosenImage2 = m_noty.pulova;
            break;
        case 7:
            m_nota2.choosenImage2 = m_noty.cela;
            break;
        }
    }
    else
    {
        switch (i)
        {
        case 0:
            m_nota1.choosenImage1 = m_noty.sestnactinova;
            break;
        case 1:
            m_nota1.choosenImage1 = m_noty.osminova;
            break;
        case 2:
            m_nota1.choosenImage1 = m_noty.ctvrtova;
            break;
        case 3:
            m_nota1.choosenImage1 = m_noty.pulova;
            break;
        case 4:
            m_nota1.choosenImage1 = m_noty.cela;
            break;
        }
        switch (j)
        {
        case 0:
            m_nota2.choosenImage2 = m_noty.sestnactinova;
            break;
        case 1:
            m_nota2.choosenImage2 = m_noty.osminova;
            break;
        case 2:
            m_nota2.choosenImage2 = m_noty.ctvrtova;
            break;
        case 3:
            m_nota2.choosenImage2 = m_noty.pulova;
            break;
        case 4:
            m_nota2.choosenImage2 = m_noty.cela;
            break;
        }
    }
    m_nota1.repaint();
    m_nota2.repaint();
}
void MainComponent::tripletButtonChanged()
{
    if (TripletsButton->getToggleState() == true)
    {
        DelkaNotSlider->setRange(0, 7, 1);
    }
    else 
    {
        DelkaNotSlider->setRange(0, 4, 1);
    }
    DelkaNotSliderChanged();
}

void MainComponent::timerCallback()
{
    if (m_analyzer.nextFFTBlockReady)
    {
        m_analyzer.showText = false;
        m_analyzer.drawNextFrameOfSpectrum();
        m_analyzer.nextFFTBlockReady = false;
        m_analyzer.repaint();        
    }
}

void MainComponent::VelikostObrazuSliderChanged()
{

    if (!m_cv.m_mat_original.empty())
    {
        fileComp->setEnabled(false);
        playButton->setEnabled(false);

        m_cv.clearAllExceptOriginal();
        m_cv.imageResizeOriginal(double2int(VelikostObrazuSlider->getValue()), double2int(VelikostObrazuSlider->getValue()));
        m_cv.RGB2HSV();
        m_cv.RGB2YCrCb();
        m_cv.RGB2HLS();
        m_cv.splitRGB();
        m_cv.splitHLS();
        m_cv.splitHSV();
        m_cv.splitYCrCb();

        // sine generator wavetable generator
        sineTable.setSize(m_cv.m_mat.rows * m_cv.m_mat.cols, (int)tableSize + 1);
        sineTable.clear();
        oscillators.clear();

        m_cv.HSV_q = m_cv.HSV;
        m_cv.RGB_q = m_cv.RGB;
        m_cv.HLS_q = m_cv.HLS;
        m_cv.YCbCr_q = m_cv.YCbCr;

        for (int index = 0; index < m_cv.m_mat.rows * m_cv.m_mat.cols; index++)
        {
            m_cv.quantizeUchar(m_cv.HSV[index].S, m_cv.HSV_q[index].S, 0, 255, 1, max_pocet_harmonickych);        // pocet harmonickych
         //   m_cv.quantizeUchar(m_cv.HSV[index].H, m_cv.HSV_q[index].H, 0, 255, 0, 1);                          // sudé, liché harmonické 
            m_cv.quantizeUchar(m_cv.YCbCr[index].Y, m_cv.YCbCr_q[index].Y, 0, 255, 0, double2float(VyskaSlider->getMaxValue() - VyskaSlider->getMinValue() + (double)1)); // vyska tonu 
           
            //m_cv.quantizeUchar(m_cv.RGB[index].B, m_cv.RGB_q[index].B, 0, 255, 5, 20);
            //m_cv.quantizeUchar(m_cv.RGB[index].G, m_cv.RGB_q[index].G, 0, 255, 5, 50);
            //m_cv.quantizeUchar(m_cv.RGB[index].R, m_cv.RGB_q[index].R, 0, 255, 1, 100); 

            m_cv.quantizeUchar(m_cv.HLS[index].L, m_cv.HLS_q[index].L, 0, 255, 0, 30);                          // hodnota světlosti

            m_cv.quantizeUchar(m_cv.HSV[index].V, m_cv.HSV_q[index].V, 0, 255, double2float(DelkaNotSlider->getMinValue()), double2float(DelkaNotSlider->getMaxValue()));  // volba délky not 

            createWavetable(index);
            createOscillators(index);
        }

        File resized(File::getCurrentWorkingDirectory().getChildFile("resized.png"));
        m_nahled.nahledImage = ImageFileFormat::loadFrom(resized);

        m_nahled.repaint();
        playButton->setEnabled(true);
        fileComp->setEnabled(true);

    }
}
void MainComponent::run()
{    
        m_cv.RGB2HSV();
        m_cv.RGB2YCrCb();
        m_cv.RGB2HLS();
        m_cv.splitRGB();
        m_cv.splitHLS();
        m_cv.splitHSV();
        m_cv.splitYCrCb();

        // sine generator wavetable generator
        sineTable.setSize(m_cv.m_mat.rows * m_cv.m_mat.cols, (int)tableSize + 1);
        sineTable.clear();
        oscillators.clear();

        m_cv.HSV_q = m_cv.HSV;
        m_cv.RGB_q = m_cv.RGB;
        m_cv.HLS_q = m_cv.HLS;
        m_cv.YCbCr_q = m_cv.YCbCr;

        for (int index = 0; index < m_cv.m_mat.rows * m_cv.m_mat.cols; index++)
        {
            m_cv.quantizeUchar(m_cv.HSV[index].S, m_cv.HSV_q[index].S, 0, 255, min_pocet_harmonickych, max_pocet_harmonickych);        // pocet harmonickych
       //     m_cv.quantizeUchar(m_cv.HSV[index].H, m_cv.HSV_q[index].H, 0, 255, 0, 1);                                                   // sudé, liché harmonické 
            m_cv.quantizeUchar(m_cv.YCbCr[index].Y, m_cv.YCbCr_q[index].Y, 0, 255, 0, double2float(VyskaSlider->getMaxValue() - VyskaSlider->getMinValue() + (double)1)); // vyska tonu 

            //m_cv.quantizeUchar(m_cv.RGB[index].B, m_cv.RGB_q[index].B, 0, 255, 5, 20); 
            //m_cv.quantizeUchar(m_cv.RGB[index].G, m_cv.RGB_q[index].G, 0, 255, 5, 50); 
            //m_cv.quantizeUchar(m_cv.RGB[index].R, m_cv.RGB_q[index].R, 0, 255, 1, 100); 

            m_cv.quantizeUchar(m_cv.HLS[index].L, m_cv.HLS_q[index].L, 0, 255, 0, 30);                          // hodnota světlosti - poměr mezi vyššími harmonickými

            m_cv.quantizeUchar(m_cv.HSV[index].V, m_cv.HSV_q[index].V, 0, 255, double2float(DelkaNotSlider->getMinValue()), double2float(DelkaNotSlider->getMaxValue()) );       // volba délky not 

            createWavetable(index);
            createOscillators(index);
        }

        if (VelikostObrazuSlider->getValue()* VelikostObrazuSlider->getValue() == oscillators.size())
        {           
            signalThreadShouldExit();
            MessageManagerLock mml;
            playButton->setEnabled(true);
            fileComp->setEnabled(true);
        }
       

}


//===============================================================================


/*
    POZNÁMKY    




*/