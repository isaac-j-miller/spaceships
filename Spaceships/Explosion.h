#pragma once
#include <SFML/Graphics.hpp>
#include "Tools.h"
#include "FrameClock.h"
#include "ScreenThing.h"
#ifndef EXPLOSION_H
#define EXPLOSION_H
class Explosion: virtual public ScreenThing
{
	static sf::Image image;
	static sf::Texture texture;
public:
	Explosion();
	Explosion(float microSecondsDuration, float size, point centerPosition);
	virtual ~Explosion() {}
	bool isActive();
	void setImage();
	sf::Sprite getSprite();
	static bool Init(const std::string& FileName);
protected:
	FrameClock clock;
	float size = 10;
	float duration = 20; //in frames
	bool active = true;
};
#endif
