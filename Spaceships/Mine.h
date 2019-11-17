#pragma once
#include "Projectile.h"
#ifndef MINE_H
#define MINE_H
class Mine :
	public Projectile
{
	static sf::Image image;
	static sf::Texture texture;
public:
	Mine(point pos, point traj, int dmg, Spaceship* f);
	~Mine();
	void setImage();
	bool move();
	static bool Init(const std::string& FileName);
};

#endif