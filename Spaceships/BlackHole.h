#pragma once
#ifndef BLACKHOLE_H

#define BLACKHOLE_H
#include "Tools.h"
#include "Moveable.h"
class BlackHole :
	public ScreenThing
{
	static sf::Texture texture;
	static sf::Image image;
public:
	static bool Init(const std::string&);
	void setImage();
	BlackHole(point);
	void eatSpaceships();
	void eatProjectiles();
	point getAccelerationVector(Moveable*);
	sf::Sprite getSprite();
private:
	
	void updateSize();
	unsigned long long int pointsAccumulated = 50000;
	const float gravitationalConstant = 1E-1;
	const float growthConstant =2E-3;
	const float initialSize = 20;
	float size = 0;
};
#endif // !1

