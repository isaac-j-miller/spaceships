#pragma once
#include "ScreenThing.h"
#include "FrameClock.h"
#ifndef MOVEABLE_H
#define MOVEABLE_H
class Moveable :
	virtual public ScreenThing
{
public:
	Moveable() {}
	virtual ~Moveable() {}
	//virtual void moveSprite() {};
	point getTrajectory();
	point getDisplacementVector() { return displacementVector; }
protected:
	float speed = 0; //in pixels per frame
	point trajectory = { 0,0 };
	point displacementVector = { 0,0 };
	FrameClock moveClock;
};
#endif
