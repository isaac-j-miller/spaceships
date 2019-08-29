#include "Projectile.h"
#include <iostream>
#include "Spaceship.h"
#include "Explosion.h"
std::vector<Explosion*>* Projectile::explosions;
std::vector<Spaceship*>* Projectile::spaceships;
point Projectile::windowSize;
Projectile::Projectile(point pos, point traj, int dmg, Spaceship* f) {
	father = f;
	clock.restart();
	position = pos;
	damage = dmg;
	fatherSpeed = f->getDisplacementVector();
	point offset = {0,0};
	trajectory = round_down(traj) + offset;
	rotation = asin(trajectory.x / magnitude(trajectory));
	//std::cout << "creating projectile with trajectory " << trajectory << std::endl;
	collisionBox = getCollisionBox();
	moveSprite();
	//position = getTopLeft(collisionBox);
	//projectileList.push_back(*this);
}
void Projectile::Init(std::vector<Explosion*>* e, std::vector<Spaceship*>* s, point window) {
	explosions = e;
	spaceships = s;
	windowSize = window;
}
void Projectile::setImage() {
	
}
int Projectile::getCounter() {
	return counter;
}
void Projectile::explode() {
	explosions->push_back(new Explosion(explosionDuration, explosionSize, getCollisionCoords()));
}
point Projectile::getAvgPosition() {
	return avgPosition;
}
void Projectile::moveSprite() {
	//sprite.setOrigin(sf::Vector2f(width / 2, height / 2));
	sprite.setRotation((vectToAngle(trajectory)-M_PI_2)*180/M_PI);
	//std::cout << "moving and rotating sprite. position is " << position << "angle is"
	//sprite.setOrigin(sf::Vector2f(0,0));
	sprite.setPosition(avgPosition.x,avgPosition.y);
}
bool Projectile::getCollision() {
	return collision;
}

box Projectile::getCollisionBox() {
	if (!(position == prevPosition)) {
		box pbox = rotate(baseTransform, getRotation() * M_PI);
			pbox = pbox + position;
			collisionBox = pbox;
			avgPosition = averagePosition(collisionBox);
			return pbox;
	}
	else {
		return collisionBox;
	}
}
point Projectile::getCollisionCoords() {
	return collisionCoords;
}
Spaceship* Projectile::getFather() {
	return father;
}
float Projectile::getRotation() {
	//std::cout << "rotation is " << magnitude(trajectory)*180 << " degrees" <<std::endl;
	//this is probably messed up
	//return magnitude(trajectory);
	
	return rotation;
}
point Projectile::getTrajectory() {
	return trajectory;
}

bool Projectile::move() {
	counter++;
	unsigned long long int timeElapsed = clock.getTime();
	if (timeElapsed != 0) {
		prevPosition = position;
		//std::cout << "Elapsed time: " << timeElapsed << "; ";
		//std::cout << "orig cBox: " << iB << std::endl;
		//std::cout << "moving from (" << position.x << ',' << position.y << ") to (";
		position = position + trajectory * (pow(timeElapsed, 2) * acceleration / 2 + speed * timeElapsed);
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
		//std::cout << position.x << ',' << position.y << ')' << std::endl;
		//std::cout << "speed: " << speed << "px/frame" << std::endl;
		//std::cout << "yspeed = " << trajectory.y * (pow(timeElapsed, 2) * acceleration / 2 + speed * timeElapsed) << " pixels/us" << std::endl;
		getCollisionBox();
		const point points[] = { collisionBox.topLeft, collisionBox.bottomLeft, collisionBox.bottomRight, collisionBox.topRight };
		for (int i = 0; i < 4;i++) {
			//temp assign 
			if (father == spaceships->at(0)) {
				for (auto s : *spaceships) {

					box c = s->getCollisionBox();
					//std::cout <<"line: "<< l << "; " << std::endl;
					if (s != father && pointDistance(position, s->getPosition()) < 1.5 * (getMaxDimension() + s->getMaxDimension())) {
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
				if (s != father && pointDistance(position, s->getPosition()) < 1.5 * (getMaxDimension() + s->getMaxDimension())) {
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
		clock.restart();
		moveSprite();
		//std::cout << "timer: " << lifeClock.getTime() << " duration: " << duration << std::endl;
		return (inRange(position, windowSize) ? true : false);
	}
	else {
		return false;
	}
}
//point Projectile::getAvgPosition() {
//	return avgPosition;
//}
point Projectile::getPosition() {
	return position;
}
float Projectile::getMaxDimension() {
	return maxDimension;
}
sf::Sprite Projectile::getSprite() {
	return sprite;
}