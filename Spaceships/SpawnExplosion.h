#pragma once
#include "Burst.h"
class SpawnExplosion :
	public Burst
{
	static sf::Image image;
	static sf::Texture texture;
public:
	SpawnExplosion(float size, point centerPosition);
	~SpawnExplosion() {}
	void setImage();
	sf::Sprite getSprite();
	static bool Init(const std::string& FileName);
private:
	float initSize = 0;
};

