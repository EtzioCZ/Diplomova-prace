#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class Obraz : public Component
{
public:
    Obraz();        
    // vykreslen� obrazu
    void paint(Graphics& g) override; 
    Image loadedImage; 
};

