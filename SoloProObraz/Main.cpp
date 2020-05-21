#include <JuceHeader.h>
#include "MainComponent.h"
#include "CV.h"
#include "SineGenerator.h"

//==============================================================================
class TestjuceApplication  : public JUCEApplication
{
public:
    //==============================================================================
    TestjuceApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }
    //==============================================================================

    void initialise (const String& commandLine) override
    {
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        mainWindow = nullptr; // deletes main window
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow(String name) : DocumentWindow(name,
                                  Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),                                                    
                                  DocumentWindow::closeButton | DocumentWindow::minimiseButton | DocumentWindow::maximiseButton)
        {
            setUsingNativeTitleBar (false);
            setContentOwned (new MainComponent(), true);
           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else 
            setResizable (true, true);
     
           #endif
            setVisible (true);
            setResizeLimits(1200, 640, 10000, 10000);
            centreWithSize(getWidth(), getHeight());
            setFullScreen(true);            
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (TestjuceApplication)