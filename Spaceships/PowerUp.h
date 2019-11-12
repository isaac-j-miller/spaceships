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
class BlackHole;
class PowerUp: virtual public ScreenThing
{
	static sf::Texture texture;
public:
	PowerUp(point pos, int type, int value);
	virtual ~PowerUp();
	void setImage();
	static bool Init(const std::string& ImageFile);
	void explode();
	point getSize();
	box getCollisionBox();
	void animate();
	virtual bool isUpgrade();
	virtual bool isActive();
	virtual void upgradeSpaceship(Spaceship* s);
	sf::Sprite getSprite();
	bool eaten = false;
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
	friend class BlackHole;
	FrameClock timer;
};

#endif // !POWERUP_H