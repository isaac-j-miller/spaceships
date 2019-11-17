#pragma once
#include "ScreenThing.h"
#include "FrameClock.h"
#ifndef BURST_H
#define BURST_H
class Burst :
	public ScreenThing
{
public:
	Burst(float framesDuration, float size, point centerPosition);
	Burst();
	virtual ~Burst(){}
	bool isActive();
	
	virtual sf::Sprite getSprite();
protected:
	point boxSize;
	point avgPosition;
	FrameClock clock;
	float size = 10;
	float duration = 20; //in frames
	bool active = true;
};

#endif BURST_H