#include "Spaceship.h"
#include "EnemySpaceship.h"
#include "PowerUp.h"
#include <iostream>


std::vector<Spaceship*>* Spaceship::spaceships;
std::vector<EnemySpaceship*>* Spaceship::enemySpaceships;
std::vector<Projectile*>* Spaceship::projectiles;
std::vector<Explosion*>* Spaceship::explosions;
std::string Spaceship::shieldImageFileName = "shield.png";
box Spaceship::windowBounds;
float Spaceship::boundAdjustment = 1;

Spaceship::Spaceship() {

}
Spaceship::Spaceship(point initPos, double initRotation) {
	rotation = initRotation;
	position = initPos;
	point tempTorpedoOrigin = { .5,0 };
	point tempBulletOrigin = { -.5,0 };
	//std::cout << "spaceship init w/ position " << position << ", rotation " << rotation*180 << " deg, address: " << this;
	setDims();
	shieldTimer.restart();
}
Spaceship::~Spaceship() {

}
void Spaceship::setDims() {
	baseTransform = { {-width / 2,-height / 2},{-height / 2,height / 2},{width / 2,height / 2},{width / 2,-height / 2} };
	updateCollisionBox();
	avgPosition = averagePosition(collisionBox);
	maxDimension = std::max(width, height);

	//std::cout << "collisionBox: " << collisionBox << std::endl;
	setImage();
}
void Spaceship::explode() {
	explosions->push_back( new Explosion(explosionDuration, getMaxDimension(), getAvgPosition()));
}
void Spaceship::destroy() {
	//std::cout << "base this: " << this << "this - 20"<< this - 20 << std::endl;
	delete this;
}
bool Spaceship::getEnemy() {
	return enemy;
}
unsigned int Spaceship::getLevel() {
	return level;
}
void Spaceship::incrementRotation(float rotationDir) {
	//float oldR = rotation;
	rotation += rotationDir * static_cast<float>(rotationIncrement);
	if (rotation < 0) {
		rotation = rotation + 2;
	}
	if (rotation > 2) {
		rotation = rotation - 2;
	}
	updateCollisionBox();
	/*
	std::cout << "collisionBox: " << collisionBox << std::endl;
	while (!boxWithin(collisionBox, windowBounds)) {
		std::cout << "adjusting.." << std::endl;
		rotation -= rotationDir * rotationIncrement*boundAdjustment;
		updateCollisionBox();
		std::cout << "collisionBox: " << collisionBox << std::endl;
	}
	std::cout << "collisionBox in bounds: " << collisionBox << std::endl;
	*/
	for (auto a : *spaceships) {
		if (a != this && pointDistance(position, a->getPosition()) <1.5*(getMaxDimension()+a->getMaxDimension()) ) {
			if (boxOverlap(collisionBox, a->getCollisionBox())) {
				rotation -= rotationDir * rotationIncrement * boundAdjustment;
				updateCollisionBox();
				break;
			}
		}
	}
	
	
	
	moveSprite();
	//std::cout << "rotation incremented by " << rotationDir * rotationIncrement<<"; new rotation is " << rotation << std::endl;
}
bool Spaceship::setRotation(float newRotation) {
	float oldRotation = rotation;
	bool set = true;
	
	rotation = newRotation;
	if (rotation < 0) {
		rotation = rotation + 2;
	}
	if (rotation > 2) {
		rotation = rotation - 2;
	}
	//point avg = averagePosition(getCollisionBox());
	
	for (auto a : *spaceships) {
		
		if (a != this && pointDistance(avgPosition, a->getAvgPosition()) < .75 * (getMaxDimension() + a->getMaxDimension())) {
			if (boxOverlap(collisionBox, a->getCollisionBox())) {
				rotation = oldRotation;
				set = false;
				break;
			}
		}
	}
	
	updateCollisionBox();
	moveSprite();
	return set;
}
void Spaceship::setImage() {
	
}
void Spaceship::fireBullet() {
	//std::cout << "Firing Bullet" << std::endl;
	//std::cout << "rotation =" << rotation * 180 << std::endl;
	bulletClock.restart();
	projectiles->push_back( new Bullet(bulletOrigin, getTrajectory(), bulletDamage, this));
}
point Spaceship:: getTrajectory() {
	return getComponents(rotation*M_PI + M_PI_2);
}
bool Spaceship::bulletReady() {
	return (bulletClock.getTime() >= bulletPeriod);
}
bool Spaceship::isAlive() {
	return (health > 0 ? true : false);
}
void Spaceship::increaseScore(unsigned int points) {
	score += points;
}
unsigned int Spaceship::getScore() {
	return score;
}
unsigned int Spaceship::getPointsValue() {
	return pointValue + score;
}
void Spaceship::takeDamage(int dmg) {
	//std::cout << "taking " << dmg << " damage. health is now " << health - dmg << std::endl;
	if (shieldUp) {
		shieldStrength -= dmg;
		if (shieldStrength <= 0) { //if the shieldStrength is now negative
			health += (shieldStrength);
			
			disableShield();
		}
	}
	else {
		health -= dmg;
	}
}
void Spaceship::fireTorpedo() {
	//std::cout << "Firing Torpedo" << std::endl;
	//std::cout << "rotation =" << rotation * 180 <<std::endl;
	torpedoClock.restart();
	projectiles->push_back( new Torpedo(torpedoOrigin, getTrajectory(), torpedoDamage, this));
}
bool Spaceship::torpedoReady() {
	return (torpedoClock.getTime() >= torpedoPeriod);
}
bool Spaceship::Init(const std::string& shieldImageFile, std::vector<Spaceship*>* s, std::vector<EnemySpaceship*>* e, std::vector<Projectile*>* p, std::vector<Explosion*>* ex, box window) {
	spaceships = s;
	enemySpaceships = e;
	projectiles = p;
	explosions = ex;
	windowBounds = window;
	shieldImageFileName = shieldImageFile;
	//bool done = (image.loadFromFile(ImageFile)&& shieldImage.loadFromFile(shieldImageFile));
	return true;
}
point Spaceship::getDisplacementVector() {
	return displacementVector;
}
int Spaceship::getHealth() {
	return health;
}
void Spaceship::move(point inputVector) {
	inputVector = normalizeVector(inputVector);
	updateCollisionBox();
	if (health > 0) {
		prevPosition = position;
		elapsedFrames = moveClock.getTime();
		position = position + inputVector * speed;
		if (inRange(position, windowBounds.bottomRight)) {
			//if wrapped
			position = getWrapped(position, windowBounds.bottomRight);
			prevPosition = position - inputVector * speed;
		}
		//disable bc wrapping
		/*
		//check if within bounds
		if (!boxWithin(collisionBox, windowBounds)) { // if the collisionbox is not fully within the bounds
			// check which direction the thing is going and figure out which edge it's closest to & block if moving closer to edge
			point direction = (avgPosition - averagePosition(windowBounds)) * inputVector;
			if (direction.x > 0 || direction.y > 0) { // moving in wrong direction
				position = prevPosition;
			}
			
		}
		*/
		//else {// if (boxWithin(collisionBox, inflate(windowBounds, .93))){ // if within bounds and 
			//iterate over other spaceships
			for (auto a : *spaceships) {
				if (a != this) {
					point aPos = a ->getAvgPosition();
					
					if (pointDistance(avgPosition, aPos) < (getMaxDimension() + a->getMaxDimension())) { // if spaceship is close
						box aBox = a->getCollisionBox();
						point direction = (avgPosition - aPos) * inputVector;
						if (boxOverlap(collisionBox, aBox)) {
							if (direction.x < 0 || direction.y < 0) { // moving in wrong direction
								position = prevPosition;
							}
							break;
						}
						else if (boxWithin(collisionBox, aBox)) {
							if (direction.x < 0 || direction.y < 0) { // moving in wrong direction
								position = prevPosition;
							}
							break;
						}
					}
				}
			}
		//}
	}
	//calculate displacement vector
	if (elapsedFrames != 0) {
		displacementVector = (position - prevPosition) * (1 / elapsedFrames);
	}
	moveSprite();
	moveClock.restart();
}
int Spaceship::getRamDamage() {
	return ramDamage;
}
float Spaceship::getMaxDimension() {
	return maxDimension;
}
bool Spaceship::pickUpBox(PowerUp* p) {
	//std::cout << "Picking up powerup" << std::endl;
	p->upgradeSpaceship(this);
	return (p->isUpgrade() ? true : false);
}
point Spaceship::getAvgPosition() {
	return avgPosition;
}
point Spaceship::getPosition() {
	return position;
}
bool Spaceship::shieldActive() {
	return shieldUp;
}
sf::Sprite Spaceship::getShield() {
	return shieldSprite;
}
void Spaceship::moveSprite() {
	sprite.setRotation(180 * rotation);
	sprite.setPosition(avgPosition.x, avgPosition.y);
	if (shieldUp) {
		shieldSprite.setRotation(180 * rotation);
		shieldSprite.setPosition(avgPosition.x, avgPosition.y);
	}
	//std::cout << "moving to: (" << position.x << ',' << position.y << ')' << std::endl;
}
void Spaceship::activateShield() {
	shieldUp = true;
	//sprite.setTexture(shielded);
}
void Spaceship::disableShield() {
	shieldUp = false;
	shieldStrength = 0;
	shieldTimer.restart();
	//sprite.setTexture(texture);
}
point Spaceship::getSize() {
	return { width,height };
}
box Spaceship::getCollisionBox() {
	if(!(position == prevPosition)){
		updateCollisionBox();
	}
	return collisionBox;
}
sf::Sprite Spaceship::getSprite() {
	return sprite;
}
void Spaceship::updateCollisionBox() {
	box pbox = rotate(baseTransform, rotation * M_PI);
	pbox = pbox + position;
	collisionBox = pbox;
	avgPosition = averagePosition(collisionBox);
	static point wh = { width, height };
	torpedoOrigin = avgPosition + rotate(wh * tempTorpedoOrigin, rotation * M_PI);
	bulletOrigin = avgPosition + rotate(wh * tempBulletOrigin, rotation * M_PI);
}