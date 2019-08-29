#pragma once
#include <SFML/Graphics.hpp>
#include "Tools.h"
#include "FrameClock.h"
//#include "SpaceThing.h"
#ifndef EXPLOSION_H
#define EXPLOSION_H
class Explosion//:
	//public SpaceThing
{
	static sf::Image image;
	static sf::Texture texture;
public:
	Explosion();
	Explosion(float microSecondsDuration, float size, point centerPosition);
	bool isActive();
	void setImage();
	sf::Sprite getSprite();
	static bool Init(const std::string& FileName);
protected:
	point position = { 0,0 };
	sf::Sprite sprite;
	FrameClock clock;
	float size = 10;
	float duration = 20; //in frames
	bool active = true;
};
#endif
