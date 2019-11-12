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
	void move(point inputVector) {Cruiser::move(inputVector); };
	~EnemyCruiser();
	void fireBullet();
	void fireTorpedo();
	void layMine();
	void updateCollisionBox();
	bool isActive() { return Cruiser::isActive(); }
};
#endif
