#include "ScreenThing.h"
#include "SpaceshipFactory.h"
#include "Collision.h"
#include "Burst.h"
std::string ScreenThing::shieldImageFileName;
std::vector<Spaceship*>* ScreenThing::spaceships;
std::vector<EnemySpaceship*>* ScreenThing::enemySpaceships;
std::vector<Projectile*>* ScreenThing::projectiles;
std::vector<Burst*>* ScreenThing::explosions;
std::vector<PowerUp*>* ScreenThing::powerUps;
box ScreenThing::windowBounds;
point ScreenThing::windowSize;
BlackHole* ScreenThing::blackHole = nullptr;

ScreenThing::ScreenThing() {
	
}

ScreenThing::~ScreenThing() {
	//internalThread.join();
}

void ScreenThing::setImage() {

}

bool ScreenThing::isActive() {
	return true;
}


sf::Sprite ScreenThing::getSprite() {
	return sprite;
}

void ScreenThing::InitGame(std::string shield, box windowB, point windowS, std::vector<Spaceship*>* s,
	std::vector<EnemySpaceship*>* e,
	std::vector<Projectile*>* p,
	std::vector<Burst*>* ex,
	std::vector<PowerUp*>* pu)
{
	shieldImageFileName = shield;
	windowBounds = windowB;
	windowSize = windowS;
	spaceships = s;
	enemySpaceships = e;
	projectiles = p;
	explosions = ex;
	powerUps = pu;
	//std::cout << windowB << std::endl << windowS << std::endl;
}


box ScreenThing::getCollisionBox() {
	//std::cout << "base class cbox" << std::endl;
	if (!(position == prevPosition)) {
		box pbox = baseTransform;
		if (oldRotation != rotation) {
			pbox = rotate(baseTransform, getRotation() * M_PI);
			pbox = pbox + position;
			
		}
		collisionBox = pbox;
		avgPosition = averagePosition(collisionBox);
		return pbox;
	}
	else {
		return collisionBox;
	}
}
void ScreenThing::setDims(int w, int h) {
	width = w;
	height = h;
	baseTransform = { {-width / 2,-height / 2},{-height / 2,height / 2},{width / 2,height / 2},{width / 2,-height / 2} };
	getCollisionBox();
	avgPosition = averagePosition(collisionBox);
	maxDimension = std::max(width, height);

	//std::cout << "collisionBox: " << collisionBox << std::endl;
	setImage();
}

point ScreenThing::rotateLocalToGlobal(point inputVector) {
	return rotate(normalizeVector(inputVector), rotation * M_PI - M_PI);
}