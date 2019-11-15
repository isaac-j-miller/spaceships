#pragma once
#include "EnemySpaceship.h"
#ifndef SPACESHIPFACTORY_H
#define SPACESHIPFACTORY_H
class SpaceshipFactory
{
	static int aggressiveness;
	static int sluggishness;
	static point windowSize;
	static std::vector<Spaceship*>* spaceships;
public:
	static void Init(int d,int slug,  point window, std::vector<Spaceship*>* s);
	static void setDifficulty(int, int);
	static Spaceship* generatePlayer(int level);
	static Spaceship* upgradePlayer(Spaceship* p);
	static EnemySpaceship* generateRandomEnemy(int level);
	static EnemySpaceship* generateRandomEnemyPosition(point pos, int level);
	static EnemySpaceship* generate(point pos, int initRotation, int level);
	
};
#endif
