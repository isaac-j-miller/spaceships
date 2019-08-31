#pragma once
#include <SFML/Graphics.hpp>
#include "Tools.h"
#include "FrameClock.h"
#include "ScreenThing.h"
#ifndef POWERUP_H
#define POWERUP_H
#define NUM_TYPES 8
class Spaceship;
class TextExplosion;
class Explosion;
class PowerUp: virtual public ScreenThing
{

	static sf::Texture texture;
protected:
	
	//static sf::Image image;
public:
	PowerUp(point pos, int type, int value);
	virtual ~PowerUp();
	void setImage();
	static bool Init(const std::string& ImageFile);
	void explode();
	//point getPosition();
	point getSize();
	box getCollisionBox();
	//point getAvgPosition();
	void animate();
	virtual bool isUpgrade();
	virtual bool isActive();
	virtual void upgradeSpaceship(Spaceship* s);
	sf::Sprite getSprite();
protected:
	int type;
	int value;
	bool collision = false;
	int pulsePeriod = 75;
	float pulseScale = .25;
	float pulseSlope = 1;
	float origSize;
	float size = 15;
	std::string explosionString;
	float stringSize = 30;
	//point avgPosition;
	//point position;
	//sf::Sprite sprite;
	FrameClock timer;
	//box collisionBox = { {0,0}, {0,0 - size}, {0 + size,0 - size}, {0 + size,0} };
};

#endif // !POWERUP_H