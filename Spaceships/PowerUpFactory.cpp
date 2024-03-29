#include "PowerUpFactory.h"
#include "Upgrade.h"
#include <chrono>
#include "BlackHole.h"
std::vector<PowerUp*>* PowerUpFactory::powerUps;
point PowerUpFactory::windowSize;
int PowerUpFactory::factor = 5;

void PowerUpFactory::Init(std::vector<PowerUp*>* powerUpsVect, point wSize) {
	powerUps = powerUpsVect;
	
	windowSize = wSize;
	//std::cout << seed << std::endl;
	
}
PowerUp* PowerUpFactory::generateSpecific(point pos, int type, int value) {
	return new PowerUp(pos, type, value);
}
PowerUp* PowerUpFactory::generateRandom() {
	bool c = false; //used to make sure RNG doesn't place 2 spaceships in the same spot
	
	
	
	point randpos;
		do {
			randpos = { ((rand() % 80 + 10) / 100.f) * (windowSize.x),((rand() % 80 + 10) / 100.f) * (windowSize.y) };
			c = false;
			if (ScreenThing::blackHole == nullptr || !pointInBox(randpos, ScreenThing::blackHole->getCollisionBox())) {
				for (auto s : *powerUps) {
					if (pointDistance(s->getPosition(), randpos) < 2.1 * s->getSize().x) {
						c = true;
					}
				}
			}
		} while (c);
		int type = rand() % NUM_TYPES;
		std::tuple<int, int> values = getValue(type);
		int minValue = std::get<0>(values);
		int maxValue = std::get<1>(values);
		int value = (rand() % (maxValue - minValue) + minValue) * factor;
		//std::cout << "New Powerup Position: " << randpos << " Type: " << type << " Value: " << value << std::endl;
		return new PowerUp(randpos, type, value);

}
PowerUp* PowerUpFactory::generateSpecificTypeRandom(int type) {
	bool c = false; //used to make sure RNG doesn't place 2 spaceships in the same spot
	point randpos;
	do {
		randpos = { ((rand() % 80 + 10) / 100.f) * (windowSize.x),((rand() % 80 + 10) / 100.f) * (windowSize.y) };
		c = false;
		for (auto s : *powerUps) {
			if (pointDistance(s->getPosition(), randpos) < 2.1 * s->getSize().x) {
				c = true;
			}
		}
	} while (c);
	//int type = rand() % NUM_TYPES;
	if (type != -1) {
		std::tuple<int, int> values = getValue(type);
		int minValue = std::get<0>(values);
		int maxValue = std::get<1>(values);

		int value = (rand() % (maxValue - minValue) + minValue) * factor;
		//std::cout << "New Powerup Position: " << randpos << " Type: " << type << " Value: " << value << std::endl;
		return new PowerUp(randpos, type, value);
	}
	else {
		return new Upgrade(randpos);
	}

}
std::tuple<int,int> PowerUpFactory::getValue(int type) {
	int minValue = 1;
	int maxValue = 5;
	factor = 5;
	switch (type)
	{
	case 0:
		// shield
		minValue = 3;
		maxValue = 6;
		break;
	case 1:
		// bullet damage upgrade
		minValue = 1;
		maxValue = 2;
		break;
	case 2:
		// torpedo damage upgrade
		minValue = 1;
		maxValue = 3;
		break;
	case 3:
		// speed upgrade
		minValue = 2;
		maxValue = 6;
		break;
	case 4:
		// health upgrade
		minValue = 8;
		maxValue = 10;
		break;
	case 5:
		// bullet ROF upgrade
		minValue = 2;
		maxValue = 4;
		break;
	case 6:
		// torpedo ROF upgrade
		minValue = 2;
		maxValue = 4;
		break;
	case 7:
		// rotation speed upgrade
		minValue = 1;
		maxValue = 2;
		break;
	case 8:
		// mine refill
		minValue = 2;
		maxValue = 6;
		factor = 1;
		break;
	case 9:
		// guided missile refill
		minValue = 5;
		maxValue = 10;
		factor = 1;
		break;
	default:
		break;
	}
	return std::make_tuple( minValue, maxValue );
}