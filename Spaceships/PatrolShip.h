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
	void move(point inputVector) { Spaceship::move(inputVector); };
	PatrolShip(point initPos, double initRotation);
	virtual ~PatrolShip();
	void fireBullet();
	void fireTorpedo();
	void layMine();
	void updateCollisionBox();
	bool isActive() { return Spaceship::isActive(); }
protected:
	static sf::Texture texture;
	static sf::Texture shieldTexture;
};
#endif PATROLSHIP_H
