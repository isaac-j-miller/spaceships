#pragma once
#include "Spaceship.h"
#ifndef FIGHTER_H
#define FIGHTER_H
class Fighter :
	virtual public Spaceship
{
public:
	static bool Init(const std::string& ImageFile);
	void setImage();
	Fighter(point initPos, double initRotation);
	virtual ~Fighter();
	void fireBullet();
	void fireTorpedo();
	void updateCollisionBox();
	bool isAlive();
protected:
	static sf::Texture texture;
	static sf::Texture shieldTexture;
	point tempTorpedoOrigin = { -.5,0 };
	point tempBulletOrigin = { .5,0 };
	//static std::string imageFile;
};
#endif
