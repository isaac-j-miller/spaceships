#pragma once
#include "EnemySpaceship.h"
#ifndef SPACESHIPFACTORY_H
#define SPACESHIPFACTORY_H
class SpaceshipFactory
{
	static int difficulty;
	static point windowSize;
	static std::vector<Spaceship*>* spaceships;
public:
	static void Init(int d, point window, std::vector<Spaceship*>* s);
	static void setDifficulty(int i);
	static Spaceship* generatePlayer(int level);
	static Spaceship* upgradePlayer(Spaceship* p);
	static EnemySpaceship* generateRandomEnemy(int level);
	static EnemySpaceship* generateRandomEnemyPosition(point pos, int level);
	static EnemySpaceship* generate(point pos, int initRotation, int d, int level);
	
};
#endif
