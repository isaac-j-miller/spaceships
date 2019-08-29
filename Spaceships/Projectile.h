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
protected:
	static std::vector<Explosion*>* explosions;
	static std::vector<Spaceship*>* spaceships;
	static point windowSize;
	
public:
	Projectile(point position, point trajectory, int damage, Spaceship* f);
	static void Init(std::vector<Explosion*>* e, std::vector<Spaceship*>* s,point window);
	void moveSprite();
	void setImage();
	float getRotation();
	sf::Sprite getSprite();
	bool move(); //returns true if out of bounds or collides
	void explode();
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
	float rotation = 0;
	int counter=0;
	int damage = 0;
	//int loops = 0; //num times it can wrap
	//int maxLoops = 2;
	float speed = 4; //in pixels per frame
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
	//FrameClock lifeClock;
};
#endif
