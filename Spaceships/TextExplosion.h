#pragma once
#include "Burst.h"
#ifndef TEXTEXPLOSION_H
#define TEXTEXPLOSION_H
class TextExplosion :
	public Burst
{
	static sf::Font font;
	static sf::Color fillColor;
public:
	static void Init(const std::string& fontFile, const sf::Color fc);
	TextExplosion(const std::string& t, const point& topLeft, const float& fs);
	~TextExplosion();
	void setImage();
	sf::Sprite getSprite() { return Burst::getSprite(); }
private:
	
	float fontSize;
	std::string text;
	sf::Text sfText;
	sf::Texture iTexture; // individual texture-- textExplosions have unique text
};

#endif