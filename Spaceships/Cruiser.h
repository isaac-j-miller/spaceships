#pragma once
#include "Spaceship.h"
#ifndef CRUISER_H
#define CRUISER_H
class Cruiser :
	virtual public Spaceship
{
public:
	static bool Init(const std::string& ImageFile);
	void setImage();
	Cruiser(point initPos, double initRotation);
	void move(point inputVector) { Spaceship::move(inputVector); };
	virtual ~Cruiser();
	void fireBullet();
	void fireTorpedo();
	void updateCollisionBox();
	bool isActive() { return Spaceship::isActive(); }
protected:
	static sf::Texture texture;
	static sf::Texture shieldTexture;
	point bulletOrigin2;
	point torpedoOrigin2;

	//const float bulletSpacingFactor2 = .3; //bullet/torpedo will spawn % of the length from the corner
	//const float torpedoSpacingFactor2 = 1.1;
	//float torpedoPosition2 = width * (1 - torpedoSpacingFactor);
	//float bulletPosition2 = width * (1 - bulletSpacingFactor);
	point tempTorpedoOrigin2 = { -.3,0 };
	point tempBulletOrigin2 = { .3,0 };
	point tempTorpedoOrigin = { -.4,0 };
	point tempBulletOrigin = { .4,0 };
};
#endif

