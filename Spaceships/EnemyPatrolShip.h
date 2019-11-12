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
	void move(point inputVector) { PatrolShip::move(inputVector); };
	void fireBullet();
	void fireTorpedo();
	void layMine();
	void updateCollisionBox();
	bool isActive() { return PatrolShip::isActive(); }

#endif