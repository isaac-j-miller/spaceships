#include "Spaceship.h"
#include "EnemySpaceship.h"
#include "PowerUp.h"
#include "Mine.h"
#include "GuidedMissile.h"
#include <iostream>


Spaceship::Spaceship() {

}
Spaceship::Spaceship(point initPos, double initRotation) {
	rotation = initRotation;
	position = initPos; 
	point tempTorpedoOrigin = { .5,0 };
	point tempBulletOrigin = { -.5,0 };
	std::cout << "spaceship init w/ position " << position << ", rotation " << rotation*180 << " deg, address: " << this <<std::endl;
	//setDims(width, height);
	shieldTimer.restart();
	displacementVector = { 0,0 };
}
Spaceship::~Spaceship() {
	setTargetNull();
}
point Spaceship::getDisplacementVector() {
	return displacementVector;
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
	oldRotation = rotation;
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
	/*
	for (auto a : *spaceships) {
		if (a != this ) {
			//boxOverlap(collisionBox, a->getCollisionBox())
			sf::Sprite aSprite = a->getSprite();
			if (sprite.getGlobalBounds().intersects(aSprite.getGlobalBounds())) {
				rotation -= rotationDir * rotationIncrement;
				updateCollisionBox();
				break;
			}
		}
	}
	*/
	
	
	moveSprite();
	//std::cout << "rotation incremented by " << rotationDir * rotationIncrement<<"; new rotation is " << rotation << std::endl;
}
bool Spaceship::setRotation(float newRotation) {
	bool set = true;
	oldRotation = rotation;
	rotation = newRotation;
	if (rotation < 0) {
		rotation = rotation + 2;
	}
	if (rotation > 2) {
		rotation = rotation - 2;
	}
	//point avg = averagePosition(getCollisionBox());
	/*
	for (auto a : *spaceships) {
		
		if (a != this) {
			sf::Sprite aSprite = a->getSprite();
			if (sprite.getGlobalBounds().intersects(aSprite.getGlobalBounds())) {
				rotation = oldRotation;
				set = false;
				break;
			}
		}
	}
	*/
	updateCollisionBox();
	moveSprite();
	return set;
}
void Spaceship::setImage() {
	
}
void Spaceship::fireBullet() {
	//std::cout << "Firing Bullet from " << bulletOrigin <<std::endl;
	//std::cout << "rotation =" << rotation * 180 << std::endl;
	bulletClock.restart();
	projectiles->push_back( new Bullet(bulletOrigin, getTrajectory(), bulletDamage, this));
}

bool Spaceship::bulletReady() {
	return (bulletClock.getTime() >= bulletPeriod);
}
bool Spaceship::isActive() {
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
	torpedoClock.restart();
	if (!numGuidedMissiles) {
		projectiles->push_back(new Torpedo(torpedoOrigin, getTrajectory(), torpedoDamage, this));
	}
	else {
		projectiles->push_back(new GuidedMissile(torpedoOrigin, getTrajectory(), torpedoDamage, this));
		numGuidedMissiles--;
	}
}
void Spaceship::layMine() {
	if (numMines) {
		mineClock.restart();
		projectiles->push_back(new Mine(avgPosition, getTrajectory(), mineDamage, this));
		numMines--;
	}
}
bool Spaceship::torpedoReady() {
	return (torpedoClock.getTime() >= torpedoPeriod);
}
bool Spaceship::mineReady() {
	return (mineClock.getTime() >= minePeriod);

}

int Spaceship::getHealth() {
	return health;
}
void Spaceship::move(point inputVector) {
	if (!enemy) {
		inputVector = rotate(normalizeVector(inputVector), rotation * M_PI - M_PI);
	}
	updateCollisionBox();
	//std::cout << "pos: " << position << collisionBox << std::endl;
	if (health > 0) {
		prevPosition = position;
		elapsedFrames = moveClock.getTime();
		static point gravAccel = { 0,0 };
		if (blackHole != nullptr) {
			gravAccel = blackHole->getAccelerationVector(this);
		}
		point accelerationDisplacement = inputVector*acceleration ;
		point totalDelta = (accelerationDisplacement +gravAccel)*0.5+ displacementVector;
		point totalDeltaMinusBlackHole = displacementVector + accelerationDisplacement * 0.5;
		if (magnitude(totalDeltaMinusBlackHole) > speed) {
			point normalizedDisplacement = normalizeVector(totalDeltaMinusBlackHole);
			totalDeltaMinusBlackHole = normalizedDisplacement * speed;
			totalDelta = totalDeltaMinusBlackHole + gravAccel * 0.5;
		}
		position = position + totalDelta;
		if (!inRange(position, windowSize)) {
			//if wrapped
			position = getWrapped(position, windowBounds.bottomRight);
			prevPosition = position - totalDelta;
		}
		
	}
	//calculate displacement vector
	if (elapsedFrames != 0) {
		displacementVector = (position - prevPosition) * (1 / elapsedFrames);
	}
	updateCollisionBox();
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

sf::Sprite Spaceship::getSprite() {
	return sprite;
}
void Spaceship::updateCollisionBox() {
	//std::cout << "basetransform:" << baseTransform << std::endl;
	float rotRadians = rotation * M_PI;
	//if(oldRotation!=rotation){
		box pbox = rotate(baseTransform, rotRadians);
		pbox = pbox + position;
		collisionBox = pbox;
	//}
	
	avgPosition = averagePosition(collisionBox);
	static point wh = { width, height };
	if (oldRotation != rotation) {
		torpedoOrigin = avgPosition + rotate(wh * tempTorpedoOrigin, rotRadians);
		bulletOrigin = avgPosition + rotate(wh * tempBulletOrigin, rotRadians);
	}
	else {
		torpedoOrigin = torpedoOrigin + position - prevPosition;
		bulletOrigin = bulletOrigin + position - prevPosition;
	}
	
	moveSprite();
}
box Spaceship::getCollisionBox() {
	//std::cout << "cbox1:" << collisionBox << std::endl;
	if ((!(position == prevPosition))|(rotation!=oldRotation)) {
		//std::cout << "updating" << std::endl;
		updateCollisionBox();
	}
	return collisionBox;
}

void Spaceship::delChasedBy(GuidedMissile* g) {
	for (auto it = chasedBy.begin(); it != chasedBy.end();) {
		if ((*it) == g) {
			it = chasedBy.erase(it);
			return;
		}
		else {
			it++;
		}
	}
}
void Spaceship::addChasedBy(GuidedMissile* g) {
	chasedBy.push_back(g);
}
void Spaceship::setTargetNull() {
	for (auto it = chasedBy.begin(); it != chasedBy.end();) {
		(*it)->target = nullptr;
		(*it)->targetAlive = false;
		it++;
	}
	
}
int Spaceship::getGuidedMissileCount() {
	return numGuidedMissiles;
}
int Spaceship::getMineCount() {
	return numMines;
}
int Spaceship::getChasedByCount() {
	return chasedBy.size();
}