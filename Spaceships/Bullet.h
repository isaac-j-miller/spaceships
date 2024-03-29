#pragma once
#include "Projectile.h"
#ifndef BULLET_H
#define BULLET_H

class Bullet :
	public Projectile
{
	static sf::Image image;
	static sf::Texture texture;
public:
	Bullet(point pos, point traj, int dmg, Spaceship* f);
	~Bullet();
	void setImage();
	bool move() { return Projectile::move(); }
	static bool Init(const std::string& FileName);
	
};
#endif // !PROJECTILE_H
