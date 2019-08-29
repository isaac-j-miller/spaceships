#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tools.h"
#include "FrameClock.h"
//#include "SpaceThing.h"
#ifndef PROJECTILE_H
#define PROJECTILE_H
class Spaceship;
class Explosion;
class Projectile//:
	//public SpaceThing//abstract class
{
	
public:
	Projectile(point position, point trajectory, int damage, Spaceship* f);
	void Init();
	void moveSprite();
	void setImage();
	float getRotation();
	sf::Sprite getSprite();
	bool move(box windowBounds, std::vector<Spaceship*> spaceships); //returns true if out of bounds or collides
	Explosion* getExplosion();
	bool getCollision();
	point getCollisionCoords();
	box getCollisionBox();
	point getAvgPosition();
	//static std::vector<Projectile> projectileList;
	point getTrajectory();
	point getPosition();
	Spaceship* getFather();
	float getMaxDimension();
	int getCounter();
protected:
	int counter=0;
	int damage = 0;
	float speed = 2; //in pixels per frame
	point fatherSpeed;
	float acceleration = 0.;
	point position = { 0,0 };
	point prevPosition = { 0,0 };
	point avgPosition = { 0,0 };
	point trajectory = { 0,0 };
	float width = 4;
	float height = 8;
	float maxDimension = std::max(width, height);
	float explosionDuration = 20;//frames
	float explosionSize = 8;
	bool collision = false;
	point collisionCoords = { 0,0 };
	box baseTransform = { {0,0},{0,height},{width,height},{width,0} };
	box collisionBox = baseTransform + position;
	Spaceship* father;
	sf::Sprite sprite;
	FrameClock clock;
};
#endif
