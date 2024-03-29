#include "PowerUp.h"
#include "TextExplosion.h"
#include "Spaceship.h"
#include "EnemySpaceship.h"
#include "SpaceshipFactory.h"

sf::Texture PowerUp::texture;

//sf::Image PowerUp::image = sf::Image();
PowerUp::PowerUp(point pos, int t, int v=0) {
	position = pos;
	type = t;
	value = v;
	//std::cout << "PowerUp instantiated at " << position << std::endl;
	collisionBox = collisionBox + position;
	avgPosition = averagePosition(collisionBox);
	pulseSlope = (size * (1. + pulseScale) - size * (1. - pulseScale)) / (.5 * pulsePeriod); // size units per frame
	//std::cout << "PowerUp pulseSlope: "<< pulseSlope << std::endl;
	origSize = size;
	timer.restart();
	switch (t) {// generate explosion string
		case 0:
			// shield
			explosionString = "Shield + ";
			break;
		case 1:
			// bullet damage upgrade
			explosionString = "Bullet Damage + ";
			break;
		case 2:
			// torpedo damage upgrade
			explosionString = "Torpedo Damage + ";
			break;
		case 3:
			// speed upgrade
			explosionString = "Speed + ";
			break;
		case 4:
			// health upgrade
			explosionString = "Health + ";
			break;
		case 5:
			// bullet RoF upgrade
			explosionString = "Bullet RoF + ";
			break;
		case 6:
			// torpedo RoF upgrade
			explosionString = "Torpedo RoF + ";
			break;
		case 7:
			// torpedo RoF upgrade
			explosionString = "Rotation Speed + ";
			break;
		case 8:
			// torpedo RoF upgrade
			explosionString = "Mines + ";
			break;
		case 9:
			// torpedo RoF upgrade
			explosionString = "Guided Missiles + ";
			break;
		default:
			explosionString = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! type " + std::to_string(t);
			break;
	}
	explosionString.append(std::to_string(v));
	setImage();
}
void PowerUp::setImage() {
	sprite.setTexture(texture);
	float xScale = size / texture.getSize().x;
	float yScale = size / texture.getSize().y;
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setPosition(avgPosition.x, avgPosition.y);
}
PowerUp::~PowerUp() {
}
bool PowerUp::isUpgrade() {
	return false;
}
bool PowerUp::Init(const std::string& ImageFile) {
	
	return texture.loadFromFile(ImageFile);
}
void PowerUp::explode() {
	//std::cout << "powerUp at " << avgPosition << "exploding. type: "<<type <<" string: " << explosionString << std::endl;
	explosions->push_back( new TextExplosion(explosionString, position, stringSize));
}

point PowerUp::getSize() {
	return { size, size };
}
box PowerUp::getCollisionBox() {
	return collisionBox;
}

void PowerUp::animate() {
	if (timer.getTime() >= pulsePeriod * 2.) {
		timer.restart();
	}
	if (timer.getTime() < pulsePeriod) { //on front half
		size += pulseSlope;
	}
	else if (timer.getTime() >= pulsePeriod && timer.getTime() <= pulsePeriod * 2.) {//on back half
		size -= pulseSlope;
	}
	//std::cout << "size: " << size << std::endl;
	//resize the collisionBox and the sprite, update position;
	point tSize = { size / 2, size / 2 };
	//position = avgPosition + tSize;
	collisionBox = { {-size / 2,-size / 2},{-size / 2,size / 2},{size / 2,size / 2},{size / 2,-size / 2} };
	collisionBox = collisionBox + avgPosition;
	float xScale = size / texture.getSize().x;
	float yScale = size / texture.getSize().y;
	sprite.setScale(sf::Vector2f(xScale, yScale));
}

bool PowerUp::isActive() {
	// check for collision 
	if (eaten) {
		return false;
	}
	if (EnemySpaceship::player != nullptr) {
		box cbox = spaceships->at(0)->getCollisionBox();
		Spaceship* player = EnemySpaceship::player;
		if (player->getSprite().getGlobalBounds().intersects(sprite.getGlobalBounds())/*boxOverlap(collisionBox, cbox)*/) {
			//if collision
			collision = true;
		}
		else if (boxWithin(collisionBox, cbox)) {
			collision = true;
		}
		else {
			collision = false;
		}
		if (collision) {
			if (!isUpgrade()) {
				upgradeSpaceship(player);
			}
		}
	}
	else {
		collision = false;
	}
	
	return !collision;
}

void PowerUp::upgradeSpaceship(Spaceship* s) {
	switch (type)
	{
	case 0:
		// shield
		//std::cout << "upgrading shield" << std::endl;
		s->shieldStrength = value;
		s->activateShield();
		break;
	case 1:
		// bullet damage upgrade
		s->bulletDamage += value;
		break;
	case 2:
		// torpedo damage upgrade
		s->torpedoDamage += value;
		break;
	case 3:
		// speed upgrade
		if (s->speed < 4) {
			s->speed += float(value) / 10;
		}
		else {
			s->health += value;
			
		}
		break;
	case 4:
		s->health += value;
		break;
	case 5:
		// bullet ROF upgrade
		if (s->bulletPeriod > 20) {
			s->bulletPeriod -= value / 10;
		}
		else {
			s->health += value;
		}
		break;
	case 6:
		// torpedo ROF upgrade
		if (s->torpedoPeriod > 15) {
			s->torpedoPeriod -= value / 10;
		}
		else {
			s->health += value;
		}
		break;
	case 7:
		// rotation speed upgrade
		if (s->rotationIncrement > .08) {
			s->rotationIncrement -= static_cast<float>(value) / 500.;
		}
		else {
			s->health += value/100;
		}
		break;
	case 8:
		// bullet damage upgrade
		s->numMines += value;
		break;
	case 9:
		// torpedo damage upgrade
		s->numGuidedMissiles += value;
		break;
	default:
		break;
	}
}

sf::Sprite PowerUp::getSprite() {
	animate();
	return sprite;
}