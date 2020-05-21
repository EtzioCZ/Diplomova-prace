#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class Obraz : public Component
{
public:
    Obraz();        
    // vykreslení obrazu
    void paint(Graphics& g) override; 
    Image loadedImage; 
};

