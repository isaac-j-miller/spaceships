#pragma once
#include "Fighter.h"
#include "EnemySpaceship.h"
#ifndef ENEMYFighter_H
#define ENEMYFighter_H
class EnemyFighter :
	public Fighter, public EnemySpaceship
{
public:
	EnemyFighter(point initPos, double initRotation, int d);
	~EnemyFighter();
	void move(point inputVector) { Fighter::move(inputVector); };
	void fireBullet();
	void fireTorpedo();
	void layMine();
	void updateCollisionBox();
	bool isActive() { return Fighter::isActive(); }
};
#endif
