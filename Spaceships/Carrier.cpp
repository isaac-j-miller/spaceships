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
Carrier::Carrier(point initPos, double initRotation, int d, int s) :
	EnemySpaceship(initPos, initRotation, d, s) {
	std::cout << "carrier init" << std::endl;
	position = initPos;
	level = 3;
	width = 60;
	height = 80;
	health = 800;
	bulletPeriod = 20;
	speed = 3;
	pointValue = 8000;
	shieldStrength = 100;
	shieldPeriod = 500;
	numMines = 100;
	numGuidedMissiles = 0;
	movePeriod -= 2;
	activateShield();
	tempTorpedoOrigin = { 0, .4 };
	tempBulletOrigin = { .4, 0 };
	setDims(width, height);
	setImage();
	
}
Carrier::~Carrier() {

}
bool Carrier::isActive() {
	bool val = Spaceship::isActive();
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
	Mini* temp = new Mini(torpedoOrigin, rotation, aggressiveness,0, this);
	spaceships->push_back(temp); // instead of firing torpedos, this generates new spaceships
	enemySpaceships->push_back(temp);
	children.push_back(temp);
	torpedoClock.restart();
}
void Carrier::layMine() {
	Spaceship::layMine();
}
void Carrier::move(point inputVector) {
	inputVector = normalizeVector(inputVector);
	updateCollisionBox();
	if (health > 0) {
		prevPosition = position;
		elapsedFrames = moveClock.getTime();
		point gravAccel = { 0,0 };
		if (blackHole != nullptr) {
			gravAccel = blackHole->getAccelerationVector(this);
		}
		position = position + inputVector * speed + gravAccel * 0.5;
		if (!inRange(position, windowSize)) {
			position = getWrapped(position, windowBounds.bottomRight);
			prevPosition = position - inputVector * speed;
		}
		
		/*
		for (auto a : *spaceships) {
			if (a != this && !(a->isMini())) { // quickly filter out minis
				point aPos = a->getAvgPosition();
				if (pointDistance(avgPosition, aPos) < (getMaxDimension() + a->getMaxDimension())) { // if spaceship is close
					box aBox = a->getCollisionBox();
					point direction = (avgPosition - aPos) * inputVector;
					if (boxOverlap(collisionBox, aBox)) {
						if (direction.x < 0 || direction.y < 0) { // moving in wrong direction
							position = prevPosition;
						}
						break;
					}
					else if (boxWithin(aBox, collisionBox)) {
						if (direction.x < 0 || direction.y < 0) { // moving in wrong direction
							position = prevPosition;
						}
						break;
					}
				}
			}
		}
		*/

	}
	//calculate displacement vector
	if (elapsedFrames != 0) {
		displacementVector = (position - prevPosition) * (1 / elapsedFrames);
	}
	moveSprite();
	moveClock.restart();
}