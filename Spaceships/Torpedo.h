#pragma once
#include "Projectile.h"
#ifndef TORPEDO_H
#define TORPEDO_H

class Torpedo :
	public Projectile
{
	static sf::Image image;
	static sf::Texture texture;
public:
	Torpedo(point pos, point traj, int dmg, Spaceship* f);
	~Torpedo();
	void setImage();
	static bool Init(const std::string& FileName);


};
#endif

