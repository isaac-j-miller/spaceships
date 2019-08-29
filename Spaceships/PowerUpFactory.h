#pragma once
#include "PowerUp.h"
#ifndef POWERUPFACTORY_H
#define POWERUPFACTORY_H
class PowerUpFactory
{
	static std::vector<PowerUp*>* powerUps;
	static point windowSize;
	static int factor;
public:
	static void Init(std::vector<PowerUp*>* powerUpsVect, point wSize);
	static PowerUp* generateSpecific(point pos, int type, int value);
	static PowerUp* generateSpecificTypeRandom(int type);
	static PowerUp* generateRandom();
private:
	static std::tuple<int,int> getValue(int type); //x is min, y is max
};
#endif
