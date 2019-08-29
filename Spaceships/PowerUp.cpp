#include "PowerUp.h"
#include "TextExplosion.h"
#include "Spaceship.h"

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
TextExplosion* PowerUp::explode() {
	//std::cout << "powerUp at " << avgPosition << "exploding. type: "<<type <<" string: " << explosionString << std::endl;
	return new TextExplosion(explosionString, position, stringSize);
}
point PowerUp::getPosition() {
	return position;
}
point PowerUp::getSize() {
	return { size, size };
}
box PowerUp::getCollisionBox() {
	return collisionBox;
}
point PowerUp::getAvgPosition() {
	return avgPosition;
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

bool PowerUp::isActive(Spaceship* player) {
	// check for collision 
	box cbox = player->getCollisionBox();

	if (boxOverlap(collisionBox, cbox)) {
		//if collision
		collision = true;
		//player->pickUpBox(this);
		
		//if I make it extend to all spaceships, code will be here
	}
	else if (boxWithin(collisionBox, cbox)) {
		collision = true;
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
		if (s->torpedoPeriod > 20) {
			s->bulletPeriod -= value / 5;
		}
		else {
			s->health += value;
		}
		break;
	case 6:
		// torpedo ROF upgrade
		if (s->torpedoPeriod > 15) {
			s->torpedoPeriod -= value / 5;
		}
		else {
			s->health += value;
		}
		break;
	default:
		break;
	}
}

sf::Sprite PowerUp::getSprite() {
	animate();
	return sprite;
}