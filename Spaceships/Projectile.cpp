#include "Projectile.h"
#include <iostream>
#include "Spaceship.h"
#include "Explosion.h"

Projectile::Projectile(point pos, point traj, int dmg, Spaceship* f) {
	father = f;
	moveClock.restart();
	position = pos;
	damage = dmg;
	fatherSpeed = f->getDisplacementVector();
	point offset = {0,0};
	trajectory = round_down(traj) + offset;
	rotation = asin(trajectory.x / magnitude(trajectory));
	
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
	explosions->push_back(new Explosion(explosionDuration, explosionSize, getCollisionCoords()));
}

void Projectile::moveSprite() {
	//sprite.setOrigin(sf::Vector2f(width / 2, height / 2));
	sprite.setRotation((vectToAngle(trajectory) - M_PI_2) * 180 / M_PI);
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
	if (timeElapsed != 0) {
		prevPosition = position;
		//std::cout << "Elapsed time: " << timeElapsed << "; ";
		//std::cout << "orig cBox: " << iB << std::endl;
		//std::cout << "moving from (" << position.x << ',' << position.y << ") to (";
		position = position + trajectory * (pow(timeElapsed, 2) * acceleration / 2 + static_cast<double>(speed) * timeElapsed);
		//position.y += trajectory.y * (pow(timeElapsed, 2) * acceleration / 2 + speed * timeElapsed);
		if (counter == 2) {
			position = fatherSpeed * timeElapsed + position;
		}
		/*
		if (inRange(position,windowBounds.bottomRight)) {
			//if wrapped
			position = getWrapped(position, windowBounds.bottomRight);
			prevPosition = position - (trajectory * (pow(timeElapsed, 2) * acceleration / 2 + speed * timeElapsed));
			loops++;
		}
		*/
		point difference = position - prevPosition;
		float mag = magnitude(difference);
		speed = mag / timeElapsed;
		displacementVector = difference *(1/ timeElapsed);
		//std::cout << '(' <<position.x << ',' << position.y << ')' << std::endl;
		//std::cout << "displacement vector: " << displacementVector << "px/frame" << std::endl;
		//std::cout << "yspeed = " << trajectory.y * (pow(timeElapsed, 2) * acceleration / 2 + speed * timeElapsed) << " pixels/us" << std::endl;
		getCollisionBox();
		const point points[] = { collisionBox.topLeft, collisionBox.bottomLeft, collisionBox.bottomRight, collisionBox.topRight };
		for (int i = 0; i < 4;i++) {
			//temp assign 
			if (father == spaceships->at(0)) {
				for (auto s : *spaceships) {

					box c = s->getCollisionBox();
					//std::cout <<"line: "<< l << "; " << std::endl;
					if (s != father && pointDistance(position, s->getPosition()) < 1.5f * (getMaxDimension() + s->getMaxDimension())) {
						//std::cout << "current spaceship ("<<i<<") to check has bbox: " << c << std::endl;
						if (pointInBox(points[i], c)) {//if collision
							// make the ship take damage
							s->takeDamage(damage);
							if (s->getHealth() <= 0) {
								father->increaseScore(s->getPointsValue());
							}
							//std::cout << "spaceship at " << s << " taking damage" << std::endl;
							// create explosion at point of intersection given priority, which comes from trajectory
							collision = true;
							collisionCoords = points[i];
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
				box c = s->getCollisionBox();
				//std::cout <<"line: "<< l << "; " << std::endl;
				if (s != father && pointDistance(position, s->getPosition()) < 1.5f * (getMaxDimension() + s->getMaxDimension())) {
					//std::cout << "current spaceship ("<<i<<") to check has bbox: " << c << std::endl;
					if (pointInBox(points[i], c)) {//if collision
						// make the ship take damage
						s->takeDamage(damage);
						if (s->getHealth() <= 0) {
							father->increaseScore(s->getPointsValue());
						}
						//std::cout << "spaceship at " << s << " taking damage" << std::endl;
						// create explosion at point of intersection given priority, which comes from trajectory
						collision = true;
						collisionCoords = points[i];
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
		moveClock.restart();
		moveSprite();
		//std::cout << "inrange: " << (inRange(position, windowSize) ? "yes" : "no") << std::endl;
		return (inRange(position, windowSize) ? true : false);
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