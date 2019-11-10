#pragma once
#include "Tools.h"
#include <vector>
#include <SFML/Graphics.hpp>

#ifndef SCREENTHING_H
#define SCREENTHING_H
class Spaceship;
class PowerUp;
class Explosion;
class Projectile;
class EnemySpaceship;

class ScreenThing
{
public:
	static std::string shieldImageFileName;
	static std::vector<Spaceship*>* spaceships;
	static std::vector<EnemySpaceship*>* enemySpaceships;
	static std::vector<Projectile*>* projectiles;
	static std::vector<Explosion*>* explosions;
	static std::vector<PowerUp*>* powerUps;
protected:
	static box windowBounds;
	static point windowSize;
public:
	static void InitGame(std::string shield, box windowB, point windowS, std::vector<Spaceship*>* s,
		std::vector<EnemySpaceship*>* e,
		std::vector<Projectile*>* p,
		std::vector<Explosion*>* ex,
		std::vector<PowerUp*>* pu);
	ScreenThing();
	virtual ~ScreenThing();
	virtual void setImage();
	virtual bool isActive();
	virtual box getCollisionBox();
	void setDims(int w, int h);
	point getPosition() { return position; }
	point getAvgPosition() { return avgPosition; }
	float getRotation() { return rotation; }
	virtual sf::Sprite getSprite();
	
protected:
	float rotation = 0;
	float oldRotation = 0;
	point avgPosition;
	point position;
	point prevPosition;
	box collisionBox;
	float width;
	float height;
	box baseTransform = { {-width / 2,-height / 2},{-height / 2,height / 2},{width / 2,height / 2},{width / 2,-height / 2} };
	float maxDimension = std::max(width, height);
	sf::Sprite sprite;
};

#endif