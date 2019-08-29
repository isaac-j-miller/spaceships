#include "Carrier.h"
#include "Mini.h"
sf::Texture Carrier::texture;
sf::Texture Carrier::shieldTexture;
bool Carrier::Init(const std::string& ImageFile) {
	shieldTexture.loadFromFile(shieldImageFileName);
	return texture.loadFromFile(ImageFile);
}
void Carrier::setImage() {
	sprite.setTexture(texture);
	shieldSprite.setTexture(shieldTexture);
	shieldSprite.setColor(sf::Color(255, 255, 255, 70));

	float xScale = 1.1 * width / texture.getSize().x;
	float yScale = 1.2 * height / texture.getSize().y;

	sprite.setScale(sf::Vector2f(xScale, yScale));
	//do the same for shield
	xScale = 1.3 * width / shieldTexture.getSize().x;
	yScale = 1.3 * height / shieldTexture.getSize().y;
	shieldSprite.setOrigin(sf::Vector2f(shieldSprite.getTexture()->getSize().x * 0.5, shieldSprite.getTexture()->getSize().y * 0.5));
	shieldSprite.setScale(sf::Vector2f(xScale, yScale));

	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setPosition(avgPosition.x, avgPosition.y);
	sprite.setRotation(180 * rotation);
	shieldSprite.setPosition(avgPosition.x, avgPosition.y);
	shieldSprite.setRotation(180 * rotation);
}
Carrier::Carrier(point initPos, double initRotation, int d) :
	EnemySpaceship(initPos, initRotation, d) {
	std::cout << "carrier init" << std::endl;
	position = initPos;
	level = 3;
	width = 60;
	height = 80;
	health = 800;
	bulletPeriod = 20;
	speed = 1.5;
	pointValue = 8000;
	shieldStrength = 100;
	activateShield();
	tempTorpedoOrigin = { 0, .4 };
	tempBulletOrigin = { .4, 0 };
	setDims();
	setImage();
	
}
Carrier::~Carrier() {

}
bool Carrier::isAlive() {
	bool val = Spaceship::isAlive();
	if (!val) {
		for (auto s : children) {
			s->health = 0;
			s->fatherAlive = false;
		}
	}
	
	return val;
}
void Carrier::updateCollisionBox() {
	Spaceship::updateCollisionBox();
	if (!shieldActive()) {
		if (shieldTimer.getTime() > shieldPeriod) {
			shieldStrength = baseShieldStrength;
			activateShield();
		}
	}
}
void Carrier::fireBullet() {
	Spaceship::fireBullet();
}
void Carrier::fireTorpedo() {
	Mini* temp = new Mini(torpedoOrigin, rotation, difficulty, this);
	spaceships->push_back(temp); // instead of firing torpedos, this generates new spaceships
	enemySpaceships->push_back(temp);
	children.push_back(temp);
	torpedoClock.restart();
}