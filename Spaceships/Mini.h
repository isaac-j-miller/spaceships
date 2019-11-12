#pragma once
#include "EnemySpaceship.h"
class Carrier;
#ifndef MINI_H
#define MINI_H
class Mini :
	public EnemySpaceship
{
	static sf::Texture texture;
	static sf::Texture shieldTexture;
public:
	friend class Carrier;
	static bool Init(const std::string& ImageFile);
	void setImage();
	void move(point inputVector);
	Mini(point initPos, double initRotation, int d, Carrier* papa);
	~Mini();
	void updateCollisionBox();
	void fireBullet();
	void fireTorpedo();
	void layMine();
	bool isActive();
private:
	bool fatherAlive = true;
	Carrier* father;
};
#endif

