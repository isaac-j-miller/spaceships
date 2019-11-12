#pragma once
#include "Tools.h"
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Torpedo.h"
#include "Explosion.h"
#include "Moveable.h"
#include "BlackHole.h"

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef SPACESHIP_H
#define SPACESHIP_H
class PowerUp;
class EnemySpaceship;
class Spaceship: virtual public Moveable
{
public:
	Spaceship();
	Spaceship(point initPos, double initRotation);
	virtual ~Spaceship();
	
	virtual void setImage();
	//static bool Init(const std::string& shieldImageFile, std::vector<Spaceship*>* s, std::vector<EnemySpaceship*>* e, std::vector<Projectile*>* p, std::vector<Explosion*>* ex, box window);// , std::vector<Projectile*> p, std::vector<Spaceship*> s, std::vector<EnemySpaceship*> e, box w);
	virtual void move(point inputVector);
	void moveSprite();
	virtual void fireBullet(); //must be virtual for different attack types
	virtual void fireTorpedo(); //same
	virtual void layMine();
	void explode();
	//point getPosition();
	point getSize();
	box getCollisionBox();
	//point getTrajectory();
	
	//point getAvgPosition();
	bool bulletReady();
	bool torpedoReady();
	bool mineReady();
	virtual bool isActive();
	int getHealth();
	void takeDamage(int dmg);
	void incrementRotation(float rotationDir);
	bool setRotation(float newRotation);
	sf::Sprite getSprite();
	sf::Sprite getShield();
	virtual void updateCollisionBox(); //must be virtual for projectile origins
	float getMaxDimension();
	bool pickUpBox(PowerUp* p);
	int getRamDamage();
	void activateShield();
	void disableShield();
	void increaseScore(unsigned int points);
	unsigned int getScore();
	unsigned int getPointsValue();
	bool shieldActive();
	unsigned int getLevel();
	bool getEnemy();
	void destroy();
	bool isMini() { return mini; }
	point getDisplacementVector();
protected:
	
	bool mini = false;
	bool enemy = false;
	unsigned int level = 0;
	friend class BlackHole;
	friend class PowerUp;
	friend class SpaceshipFactory;
	unsigned int score = 0;
	unsigned int pointValue = 1000;
	bool shieldUp = false;
	int shieldStrength = 0;
	float rotationIncrement = .02;
	int ramDamage = 0;
	int bulletDamage = 10;
	int torpedoDamage = 20;
	int mineDamage = 100;
	int bulletPeriod = 10; //in frames
	int torpedoPeriod = 25;
	int minePeriod = 50; //in frames
	int explosionDuration = 50; //in frames
	int health = 100;
	point bulletOrigin;
	point torpedoOrigin;

	point tempTorpedoOrigin = { -.5,0 };
	point tempBulletOrigin = { .5,0 };
	
	sf::Sprite shieldSprite;
	sf::Vector2f sizeVector;
	FrameClock bulletClock;
	FrameClock torpedoClock;
	FrameClock mineClock;
	
	point displacementVector = { 0,0 };
	
	sf::IntRect rect = sf::IntRect(0, 0, int(width), int(height));
	point prevPosition = { 0,0 };
	int elapsedFrames = 1;
	int shieldPeriod = 2000; //frames to wait to activate shield
	int baseShieldStrength = 50;
	FrameClock shieldTimer;
};
#endif // !SPACESHIP_H
