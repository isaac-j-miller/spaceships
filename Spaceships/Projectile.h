#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tools.h"
#include "FrameClock.h"
#include "Moveable.h"
//#include "SpaceThing.h"
#ifndef PROJECTILE_H
#define PROJECTILE_H

class Spaceship;
class Explosion;
class BlackHole;
class Projectile: virtual public Moveable
{
	
public:
	Projectile(point position, point trajectory, int damage, Spaceship* f);
	
	void moveSprite();
	void setImage();
	sf::Sprite getSprite();
	virtual bool move(); //returns true if out of bounds or collides
	void explode();
	bool getCollision();
	point getCollisionCoords();
	Spaceship* getFather();
	float getMaxDimension();
	int getCounter();

protected:
	float rotationDeg;
	int counter = 0;
	int damage = 0;
	bool eaten = false;
	point fatherSpeed;
	float currentSpeed=0;
	float acceleration = 0.;
	float explosionDuration = 20;//frames
	float explosionSize = 8;
	bool collision = false;
	friend class BlackHole;
	point collisionCoords = { 0,0 };
	unsigned long int lifetime = 1000; //number of frames before destruction
	Spaceship* father;
};
#endif
