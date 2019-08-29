#pragma once
#include "Spaceship.h"
#ifndef PATROLSHIP_H
#define PATROLSHIP_H
class PatrolShip :
	virtual public Spaceship
{
	
public:
	
	static bool Init(const std::string& ImageFile);
	void setImage();
	PatrolShip(point initPos, double initRotation);
	virtual ~PatrolShip();
	//void setImage();
	void fireBullet();
	void fireTorpedo();
	void updateCollisionBox();
	bool isAlive();
protected:
	static sf::Texture texture;
	static sf::Texture shieldTexture;
};
#endif PATROLSHIP_H
