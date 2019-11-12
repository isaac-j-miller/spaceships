#pragma once
#include "EnemySpaceship.h"
#ifndef CARRIER_H
#define CARRIER_H
class Mini;
class Carrier :
	public EnemySpaceship
{
	static sf::Texture texture;
	static sf::Texture shieldTexture;
public:
	friend class Mini;
	static bool Init(const std::string& ImageFile);
	void setImage();
	void move(point inputVector);
	Carrier(point initPos, double initRotation, int d);
	~Carrier();
	bool isActive();
	void updateCollisionBox();
	void fireBullet();
	void fireTorpedo();
	void layMine();
private:
	std::vector<Mini*> children;
	
	
};
#endif

