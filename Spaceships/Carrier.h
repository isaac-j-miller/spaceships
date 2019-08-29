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
	Carrier(point initPos, double initRotation, int d);
	~Carrier();
	bool isAlive();
	void updateCollisionBox();
	void fireBullet();
	void fireTorpedo();
private:
	std::vector<Mini*> children;
	
	
};
#endif

