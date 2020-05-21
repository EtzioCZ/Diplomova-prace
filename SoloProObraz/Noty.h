#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class Noty : public Component
{
public:
	Noty();
	Image cela;
	Image pulova;
	Image pulovaTriola;
	Image ctvrtova;
	Image ctvrtovaTriola;
	Image osminova;
	Image osminovaTriola;
	Image sestnactinova;
}
;
class Nota1 : public Component
{
public:
	Nota1();
	void paint(Graphics& g) override;
	Image choosenImage1;
};

class Nota2 : public Component
{
public:
	Nota2();
	void paint(Graphics& g) override;
	Image choosenImage2;
};
