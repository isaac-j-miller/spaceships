#pragma once
#include "Explosion.h"
class TextExplosion :
	public Explosion
{
	static sf::Font font;
	static sf::Color fillColor;
public:
	static void Init(const std::string& fontFile, const sf::Color fc);
	TextExplosion(const std::string& t, const point& topLeft, const float& fs);
	~TextExplosion();
	void setImage();
private:
	point boxSize;
	point avgPosition;
	float fontSize;
	std::string text;
	sf::Text sfText;
	sf::Texture iTexture; // individual texture-- textExplosions have unique text
};

