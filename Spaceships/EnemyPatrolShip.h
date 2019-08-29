#pragma once
#include "PatrolShip.h"
#include "EnemySpaceship.h"
#ifndef ENEMYPATROLSHIP_H
#define ENEMYPATROLSHIP_H
class EnemyPatrolShip :
	public PatrolShip, public EnemySpaceship
{
public:
	EnemyPatrolShip(point initPos, double initRotation, int d);
	~EnemyPatrolShip();
	void fireBullet();
	void fireTorpedo();
	void updateCollisionBox();
	bool isAlive();
	//static bool Init();
	//void setImage();
};

#endif