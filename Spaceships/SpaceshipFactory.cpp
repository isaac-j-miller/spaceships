#include "SpaceshipFactory.h"
#include "EnemyPatrolShip.h"
#include "EnemyFighter.h"
#include "EnemyCruiser.h"
#include "Carrier.h"
//#include <chrono>
int SpaceshipFactory::difficulty;
point SpaceshipFactory::windowSize;
std::vector<Spaceship*>* SpaceshipFactory::spaceships;

void SpaceshipFactory::Init(int d, point window, std::vector<Spaceship*>* s) {
	difficulty = d;
	windowSize = window;
	spaceships = s;
	
}
void SpaceshipFactory::setDifficulty(int i) {
	difficulty = i;
}

Spaceship* SpaceshipFactory::upgradePlayer(Spaceship* p) {
	//make a "copy" of the spaceship as the next level up
	int level = p->getLevel() + 1;
	Spaceship* player = generatePlayer(level);
	//copy positional attributes
	player->position = p->position;
	player->prevPosition = p->prevPosition;
	player->displacementVector = p->displacementVector;
	player->rotation = p->rotation;
	player->elapsedFrames = p->elapsedFrames;
	player->updateCollisionBox();
	//do shield stuff: activate and set strength
	if (p->shieldActive()) {
		player->shieldStrength = p->shieldStrength;
		player->activateShield();
	}

	//copy upgradable attributes
	Spaceship* temp = generatePlayer(level - 1);
	player->speed += (p->speed - temp->speed);
	player->rotationIncrement += (p->rotationIncrement - temp->rotationIncrement);
	player->bulletDamage += (p->bulletDamage - temp->bulletDamage);
	player->torpedoDamage += (p->torpedoDamage - temp->torpedoDamage);
	player->bulletPeriod += (p->bulletPeriod - temp->bulletPeriod);
	player->torpedoPeriod += (p->torpedoPeriod - temp->torpedoPeriod);
	player->health += (p->health - temp->health);
	player->score += (p->score - temp->score);
	player->ramDamage += (p->ramDamage - temp->ramDamage);
	
	delete temp;
	//delete p;
	//spaceships->erase(spaceships->begin(), spaceships->begin() + 1);
	
	return player;
}

EnemySpaceship* SpaceshipFactory::generateRandomEnemy(int level) {
	point randpos;
	bool c = false;
	//std::cout << "generating enemy" << std::endl;
	do {
		randpos = { ((rand() % 80 + 10) / 100.f) * (windowSize.x),((rand() % 80 + 10) / 100.f) * (windowSize.y) };

		c = false;
		//std::cout << "Temp pos: " << randpos << std::endl;
		if (ScreenThing::blackHole ==nullptr||!pointInBox(randpos, inflate(ScreenThing::blackHole->getCollisionBox(),1.5))) {
			for (auto s : *spaceships) {
				//std::cout << "distance: " << pointDistance(s->getAvgPosition(), randpos) << " max dim: " << s->getMaxDimension() << std::endl;
				if (pointDistance(s->getAvgPosition(), randpos) < 2.1 * s->getMaxDimension()) {
					c = true;
						//std::cout << "Collision" << std::endl;
					break;
				}
			}
		}
	} while (c);
	return generate(randpos, 0., difficulty, level);
}
EnemySpaceship* SpaceshipFactory::generateRandomEnemyPosition(point pos, int level) {
	return generate(pos, 0., difficulty, level);
}
Spaceship* SpaceshipFactory::generatePlayer(int level) {
	
	point pos = { windowSize.x / 2, windowSize.y - 50 };
	switch (level) {
	case 0:
		return new PatrolShip(pos, 1.);
		break;
	case 1:
		return new Fighter(pos, 1.);
		break;
	case 2:
		return new Cruiser(pos, 1.);
		break;
	default:
		return new Spaceship(pos, 1.);
	}
}

EnemySpaceship* SpaceshipFactory::generate(point pos, int initRotation, int d, int level) {
	switch (level) {
	case 0:
		std::cout << "generating enemy patrol ship at " << pos << std::endl;
		return new EnemyPatrolShip(pos, 0., difficulty);
		break;
	case 1:
		std::cout << "generating enemy fighter at " << pos << std::endl;
		return new EnemyFighter(pos, 0, difficulty);
		break;
	case 2:
		std::cout << "generating enemy cruiser at " << pos << std::endl;
		return new EnemyCruiser(pos, 0, difficulty);
		break;
	case 3:
		std::cout << "generating enemy cruiser at " << pos << std::endl;
		return new Carrier(pos, 0, difficulty);
		break;
	default:
		return new EnemySpaceship(pos, 0., difficulty);
		break;
	}
}