#include "Noty.h"

Noty::Noty()
{
	cela = ImageFileFormat::loadFrom(BinaryData::cela_png, BinaryData::cela_pngSize);
	pulova = ImageFileFormat::loadFrom(BinaryData::pulova_png, BinaryData::pulova_pngSize);
	pulovaTriola = ImageFileFormat::loadFrom(BinaryData::pulovatriola_png, BinaryData::pulovatriola_pngSize);
	ctvrtova = ImageFileFormat::loadFrom(BinaryData::ctvrtova_png, BinaryData::ctvrtova_pngSize);
	ctvrtovaTriola = ImageFileFormat::loadFrom(BinaryData::ctvrtovatriola_png, BinaryData::ctvrtovatriola_pngSize);
	osminova = ImageFileFormat::loadFrom(BinaryData::osminova_png, BinaryData::osminova_pngSize);
	osminovaTriola = ImageFileFormat::loadFrom(BinaryData::osminovatriola_png, BinaryData::osminovatriola_pngSize);
	sestnactinova = ImageFileFormat::loadFrom(BinaryData::sestnactinova_png, BinaryData::sestnactinova_pngSize);
}

Nota1::Nota1()
{
}

void Nota1::paint(Graphics& g)
{
	g.drawImageWithin(choosenImage1, 0, 0, getWidth(), getHeight(), RectanglePlacement::onlyReduceInSize);
}

Nota2::Nota2()
{
}

void Nota2::paint(Graphics& g)
{
	g.drawImageWithin(choosenImage2, 0, 0, getWidth(), getHeight(), RectanglePlacement::onlyReduceInSize);
}