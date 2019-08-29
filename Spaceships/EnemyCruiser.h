#pragma once
#include "Cruiser.h"
#include "EnemySpaceship.h"
#ifndef ENEMYCRUISER_H
#define ENEMYCRUISER_H
class EnemyCruiser :
	public Cruiser, public EnemySpaceship
{
public:
	EnemyCruiser(point initPos, double initRotation, int d);
	~EnemyCruiser();
	void fireBullet();
	void fireTorpedo();
	void updateCollisionBox();
	bool isAlive();
};
#endif
