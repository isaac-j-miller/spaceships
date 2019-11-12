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
	void eatPowerUps();
	point getAccelerationVector(Moveable*);
	float getDistance(Moveable*);
	float getAngle(Moveable*);
	unsigned long int getMass();
	sf::Sprite getSprite();
private:
	void updateSize();
	unsigned long long int pointsAccumulated = 50000;
	const float gravitationalConstant = 2E-1;
	const float growthConstant =0.05;
	const float initialSize = 20;
	const float angularMomentum = 100;
	const float projectilePoints = 10;
	float polarMomentOfInertia = 1;
	float rotationSpeed = 0; //angular velocity
	float size = 0;
};
#endif // !1

