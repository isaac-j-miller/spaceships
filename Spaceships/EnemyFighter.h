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
	void updateCollisionBox();
	bool isAlive();
	//static bool Init();
	//void setImage();
};
#endif
