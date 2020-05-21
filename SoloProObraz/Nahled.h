#pragma once
#include <Juceheader.h>
#include "Utils.h"

class Nahled :public Component
{
public:
    Nahled() {setPaintingIsUnclipped(true);};

    Image nahledImage;

    // vykreslení náhledu
    void paint(Graphics& g) override
    {
        g.setColour(Colours::white);
        g.fillRect(getLocalBounds());
        g.setColour(findColour(16799488));
        g.fillRect(2, 2, getLocalBounds().getWidth() - 4, getLocalBounds().getHeight() - 4);
        g.setColour(Colours::white);
        g.setFont(23.0f);
        g.drawFittedText("Preview\n of segments", getLocalBounds(), Justification::centred, 3);

        if (nahledImage.isValid())
        {
            g.setColour(findColour(16799488));
            g.fillRect(2, 2, getLocalBounds().getWidth() - 4, getLocalBounds().getHeight() - 4);

            g.drawImageWithin(nahledImage, 2, 2, getWidth()-4, getHeight()-4, RectanglePlacement::doNotResize,false); //  RectanglePlacement::xMid           
        }
    }
};

