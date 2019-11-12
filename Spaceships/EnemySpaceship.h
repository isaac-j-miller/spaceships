#pragma once
#include "Spaceship.h"
#ifndef ENEMYSPACESHIP_H
#define ENEMYSPACESHIP_H
class EnemySpaceship :
	virtual public Spaceship
{
	
public:
	static Spaceship* player;
	EnemySpaceship(point initPos, double initRotation, int d);
	~EnemySpaceship();
	void move(point inputVector) { Spaceship::move(inputVector); };
	static void Init(Spaceship* p);
	virtual void updateCollisionBox();
	virtual void fireBullet();
	virtual void fireTorpedo();
	virtual void layMine();
	bool isActive() { return Spaceship::isActive(); }
	void specialMove();
	void attack();
	point getMoveVector();
protected:
	int difficulty = 10;
	FrameClock moveTimer;
	float moveMod = 1;
	int movePeriod = 0;
	point moveVector = {0,0};
};
#endif

