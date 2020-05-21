#include "Obraz.h"

Obraz::Obraz()
{
	setPaintingIsUnclipped(true);
}

void Obraz::paint(Graphics& g)
{
    g.setColour(Colours::white);
    g.fillRect(getLocalBounds());
    g.setColour(findColour(16799488));
    g.fillRect(2, 2, getLocalBounds().getWidth() - 4, getLocalBounds().getHeight() - 4);
    g.setColour(Colours::white);
    g.setFont(45.0f);
    g.drawFittedText("!! Welcome in this aplication !! \n\n Load your picture to play with button (Load file)", getLocalBounds(), Justification::centred, 3);

    if (loadedImage.isValid())
    {
        g.setColour(findColour(16799488));
        g.fillRect(getLocalBounds());
        g.drawImageWithin(loadedImage, 0, 0, getWidth(), getHeight(), RectanglePlacement::onlyReduceInSize); //  RectanglePlacement::xMid 
    }
}

