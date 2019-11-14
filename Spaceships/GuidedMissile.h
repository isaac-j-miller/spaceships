#pragma once
#include "Projectile.h"
#ifndef GUIDED_MISSILE_H
#define GUIDED_MISSILE_H
class Spaceship;
class GuidedMissile :
	public Projectile
{
	static sf::Image image;
	static sf::Texture texture;
public:
	GuidedMissile(point pos, point traj, int dmg, Spaceship* f);
	~GuidedMissile();
	void setImage();
	bool move();
	static bool Init(const std::string& FileName);
protected:
	point prevVector = { 0,0 };
	point targetPos = { 0,0 };
	float maxAngle = M_PI_4;
	Spaceship* target = nullptr;
	bool targetAlive = false;
	friend class Spaceship;

};
#endif

