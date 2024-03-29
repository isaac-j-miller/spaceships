#include "Projectile.h"
#include <iostream>
#include "Spaceship.h"
#include "Explosion.h"
#include "BlackHole.h"
#include "Collision.h"

Projectile::Projectile(point pos, point traj, int dmg, Spaceship* f) {
	father = f;
	moveClock.restart();
	position = pos;
	damage = dmg;
	fatherSpeed = f->getDisplacementVector();
	//std::cout << "creating projectile with initial speed " << fatherSpeed << std::endl;
	windowSize = ScreenThing::windowSize;
	//point offset = {1,1};
	trajectory = round_down(traj);
	//rotation = asin(trajectory.x / magnitude(trajectory));
	rotation = vectToAngle(trajectory);
	//std::cout << "rotation: " << vectToAngle(trajectory) << std::endl;
	rotationDeg = rotation*180.f/static_cast<float>(M_PI) + 90;
	displacementVector = trajectory;
	//std::cout << "angle: " << rotationDeg<< std::endl;
	//std::cout << "creating projectile with trajectory " << trajectory << std::endl;
	//std::cout << '(' << position.x << ',' << position.y << ')' << std::endl;
	collisionBox = getCollisionBox();
	moveSprite();
	//position = getTopLeft(collisionBox);
	//projectileList.push_back(*this);
}

void Projectile::setImage() {
	
}
int Projectile::getCounter() {
	return counter;
}
void Projectile::explode() {
	explosions->push_back(new Explosion(explosionDuration, explosionSize, getCollisionCoords(), explosionDamage));
}

void Projectile::moveSprite() {
	//sprite.setOrigin(sf::Vector2f(width / 2, height / 2));
	//std::cout << "angle: " << (vectToAngle(trajectory))<< std::endl;
	sprite.setRotation(rotationDeg);
	//std::cout << "moving sprite. position is " << position << std::endl;
	//sprite.setOrigin(sf::Vector2f(0,0));
	sprite.setPosition(avgPosition.x,avgPosition.y);
}
bool Projectile::getCollision() {
	return collision;
}


point Projectile::getCollisionCoords() {
	return collisionCoords;
}
Spaceship* Projectile::getFather() {
	return father;
}


bool Projectile::move() {
	counter++;
	unsigned long long int timeElapsed = moveClock.getTime();
	if (eaten || collision) {
		return true;
	}
	if (timeElapsed != 0) {
		point gravAccel = { 0,0 };
		if (blackHole != nullptr) {
			gravAccel = blackHole->getAccelerationVector(this);
		}
		prevPosition = position;
		position = position + gravAccel*((float)pow(timeElapsed, 2)/ 2) + normalizeVector(displacementVector) * ((float)pow(counter, 2) * acceleration / 2 + speed * timeElapsed);
		
		if (counter == 2 || speed < FP_0) {
			position = fatherSpeed * timeElapsed + position;
		}
		point difference = position - prevPosition;
		float mag = magnitude(difference);
		//currentSpeed = mag / timeElapsed;
		displacementVector = difference *(1/ timeElapsed);
		rotation = vectToAngle(displacementVector);
		//std::cout << "rotation: " << vectToAngle(trajectory) << std::endl;
		rotationDeg = rotation * 180.f / static_cast<float>(M_PI) + 90;
		getCollisionBox();
		
		if (counter > lifetime) {
			collision = true;
			collisionCoords = avgPosition;
			return true;
		}
		if (father == spaceships->at(0)) {
			for (auto s : *spaceships) {
				sf::Sprite sSprite = s->getSprite();
				
				if (s != father/* && pointDistance(position, s->getPosition()) < 1.5f * (getMaxDimension() + s->getMaxDimension())*/) {
					
					if (Collision::PixelPerfectTest(sprite, sSprite)) {//if collision
						// make the ship take damage
						s->takeDamage(damage);
						if (s->getHealth() <= 0) {
							father->increaseScore(s->getPointsValue());
						}
						
						// create explosion at point of intersection given priority, which comes from trajectory
						collision = true;
						collisionCoords = avgPosition;
						//std::cout << "Collision at " << collisionCoords << std::endl;
						return true;
						//std::cout << std::endl << "done with this spaceship. Collision true" << std::endl;
					}
					else {
						//std::cout << std::endl << "done with this spaceship. Collision false" << std::endl;
					}
				}
			}
		}
		else {
			auto s = spaceships->at(0);
			sf::Sprite sSprite = s->getSprite();
			
			if (s != father) {
				
				if (Collision::PixelPerfectTest(sprite, sSprite) ){//if collision
					// make the ship take damage
					s->takeDamage(damage);
					if (s->getHealth() <= 0) {
						father->increaseScore(s->getPointsValue());
					}
					//std::cout << "spaceship at " << s << " taking damage" << std::endl;
					collision = true;
					collisionCoords = avgPosition;
					//std::cout << "Collision at " << collisionCoords << std::endl;
					return true;
					//std::cout << std::endl << "done with this spaceship. Collision true" << std::endl;
				}
				else {
					//std::cout << std::endl << "done with this spaceship. Collision false" << std::endl;
				}
			}
		}
		//}
		moveClock.restart();
		moveSprite();
		//std::cout << "coords: " << position << ", inrange: " << (inRange(position, windowSize) ? "yes" : "no") << std::endl;
		return !inRange(position, windowSize);
	}
	else {
		return false;
	}
}

float Projectile::getMaxDimension() {
	return maxDimension;
}
sf::Sprite Projectile::getSprite() {
	return sprite;
}

void Projectile::collideWithMine() {
	collision = true;
	collisionCoords = avgPosition;
}