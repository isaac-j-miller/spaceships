#pragma once
#include <SFML/Graphics.hpp>
#include "Tools.h"
#include "FrameClock.h"
#include "Burst.h"
#ifndef EXPLOSION_H
#define EXPLOSION_H
class Explosion: virtual public Burst
{
	static sf::Image image;
	static sf::Texture texture;
public:
	Explosion(float microSecondsDuration, float size, point centerPosition, int dmg);
	~Explosion() {}
	void setImage();
	sf::Sprite getSprite();
	static bool Init(const std::string& FileName);
protected:
	
	int damagePerFrame = 0;
	
};
#endif
