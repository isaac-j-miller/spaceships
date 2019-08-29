#pragma once
#include "PowerUp.h"
#ifndef UPGRADE_H
#define UPGRADE_H
class Upgrade :
	public PowerUp
{
	static sf::Texture upgradeTexture;
public:
	Upgrade(point pos);
	~Upgrade();
	static void Init(const std::string& fileName);
	bool isUpgrade();
	void setImage();
	void upgradeSpaceship(Spaceship* s);
	bool isActive(Spaceship* player);
};
#endif

