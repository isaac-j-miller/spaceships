#include "Spaceship.h"
#include "EnemySpaceship.h"
#include "PowerUp.h"
#include "Mine.h"
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
}
Spaceship::~Spaceship() {

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
	//std::cout << "Firing Torpedo" << std::endl;
	//std::cout << "rotation =" << rotation * 180 <<std::endl;
	torpedoClock.restart();
	projectiles->push_back( new Torpedo(torpedoOrigin, getTrajectory(), torpedoDamage, this));
}
void Spaceship::layMine() {
	//std::cout << "Firing Torpedo" << std::endl;
	//std::cout << "rotation =" << rotation * 180 <<std::endl;
	mineClock.restart();
	projectiles->push_back(new Mine(avgPosition, getTrajectory(), mineDamage, this));
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
	inputVector = normalizeVector(inputVector);
	updateCollisionBox();
	//std::cout << "pos: " << position << collisionBox << std::endl;
	if (health > 0) {
		prevPosition = position;
		elapsedFrames = moveClock.getTime();
		point gravAccel = { 0,0 };
		if (blackHole != nullptr) {
			gravAccel = blackHole->getAccelerationVector(this);
		}
		position = position + inputVector * speed +gravAccel*0.5;
		if (!inRange(position, windowSize)) {
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
		//std::cout << "s projectiles:" << projectiles->size() << std::endl;
		
		/*
		for (auto a : *spaceships) {
			if (a != this) {
				point aPos = a->getAvgPosition();

				if (pointDistance(avgPosition, aPos) < (getMaxDimension() + a->getMaxDimension())) { // if spaceship is close
					box aBox = a->getCollisionBox();

					point direction = (avgPosition - aPos) * inputVector;
					if (getSprite().getGlobalBounds().intersects(a->getSprite().getGlobalBounds())) {
						static int signs[] = { -1,1 };
						int index = rand() % 2 - 1;
						int sign = signs[index];
						std::cout << direction << std::endl;
						if (direction.x < 0 || direction.y < 0) { // moving in wrong direction
							position = prevPosition + normalizeVector(direction) * speed * sign;
							std::cout << "case a, vect=" << normalizeVector(direction) * speed * sign << std::endl;
								
						}
						if (abs(direction.x) <= FP_0 && abs(direction.y) <= FP_0) {
							position = prevPosition + normalizeVector(direction) * speed * sign;
							std::cout << "case b, vect=" << normalizeVector(direction) * speed * sign << std::endl;
								
						}
						break;
					}

					
				}
			}
		}
		*/
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
	if(oldRotation!=rotation){
		box pbox = rotate(baseTransform, rotRadians);
		pbox = pbox + position;
		collisionBox = pbox;
	}
	
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