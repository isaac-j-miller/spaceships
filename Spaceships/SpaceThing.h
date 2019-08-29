#pragma once
#include "Tools.h"
#include <SFML/Graphics.hpp>
#include <vector>
#ifndef SPACETHING_H
#define SPACETHING_H
class Spaceship;
class EnemySpaceship;
class Projectile;
class Explosion;
class SpaceThing
{
	
	static sf::Image background;
	static sf::Texture backgroundTexture;
	static sf::Sprite bSprite;
	static sf::Font font;
	static sf::Text healthReadout;
	static long long unsigned int frame;
	static float framerate; //fps
	static float framePeriodMicroSeconds;
	
public:
	static sf::RenderWindow window;
	static box windowBounds;
	static std::vector<Spaceship*> spaceships;
	static std::vector<EnemySpaceship*> enemySpaceships;
	static std::vector<Projectile*> projectiles;
	static std::vector<Explosion*> explosions;
	static Spaceship* player;
	static sf::Clock masterClock;
	
	static void Inititiate(point size, int numEnemies, int difficulty);
	static void draw();
	static void getUserInput();
	static void doFrame();
	point getPosition();
	box getCollisionBox();
	point getAvgPosition();
	sf::Sprite getSprite();
	void setImage();
	void moveSprite();
	float getMaxDimension();
	float getRadius();
	float getRotation();

protected:
	float height;
	float width;
	float maxDimension;
	float radius;
	float rotation;
	point avgPosition;
	sf::Sprite sprite;
	box baseTransform = { {-width / 2,-height / 2},{-height / 2,height / 2},{width / 2,height / 2},{width / 2,-height / 2} };
	point position = { 0,0 };
	point prevPosition = { 0,0 };
	box collisionBox = { {0,0}, {0,0 - height}, {0 + width,0 - height}, {0 + width,0} };
};
#endif

