#include "GuidedMissile.h"
#include "BlackHole.h"
#include "Collision.h"
#include "Spaceship.h"
#include "EnemySpaceship.h"
sf::Image GuidedMissile::image = sf::Image();
sf::Texture GuidedMissile::texture = sf::Texture();
bool GuidedMissile::Init(const std::string& FileName) {
	return (image.loadFromFile(FileName) &&
		texture.loadFromImage(image));
}
GuidedMissile::~GuidedMissile() {
	//std::cout << "del GuidedMissile" << std::endl;
	if (targetAlive) {
		target->delChasedBy(this);
	}
}
GuidedMissile::GuidedMissile(point pos, point traj, int dmg, Spaceship* f) :
	Projectile(pos, traj, dmg, f) {
	//std::cout << "new GuidedMissile" << std::endl;
	speed = 5; //in pixels per frame
	explosionDuration = 10;//frames
	explosionSize = 30;
	explosionDamage = 1;
	width = 10;
	height = 15;
	
	acceleration = 0.6; // pixels/(frame^2)

	setDims(width, height);
	setImage();
	float distance = 10000;
	if (father == EnemySpaceship::player) {
		for (auto a : *spaceships) {
			if (a != father) {
				point difference = a->getAvgPosition() - avgPosition;
				float diffMag = magnitude(difference);
				/*abs(father->getRotation() * M_PI + M_PI_2 - vectToAngle(difference)) < maxAngle*/
				if (diffMag < distance && pointInRangeOfAngle(father->getAvgPosition(),father->getTrajectory(),maxAngle, a->getAvgPosition())) {
					target = a;
					targetAlive = true;
					distance = diffMag;
				}
			}
		}
	}
	else if(pointInRangeOfAngle(father->getAvgPosition(), father->getTrajectory(), maxAngle, EnemySpaceship::player->getAvgPosition())) {
		target = EnemySpaceship::player;
		targetAlive = true;
	}
	else {
		target = nullptr;
		targetAlive = false;
	}
	if (targetAlive) {
		target->addChasedBy(this);
	}
}
void GuidedMissile::setImage() {
	sprite.setTexture(texture);
	sprite.setRotation(getRotation());
	float xScale = 1 * width / image.getSize().x;
	float yScale = 1 * height / image.getSize().y;
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setPosition(avgPosition.x, avgPosition.y);

}
bool GuidedMissile::move() {
	counter++;
	point moveVector = { 0,0 };
	unsigned long long int timeElapsed = moveClock.getTime();

	if (eaten) {
		return true;
	}
	if (timeElapsed != 0) {
		
		if (targetAlive) {
			
			targetPos = target->getAvgPosition();
			point differenceVector = normalizeVector(targetPos - avgPosition);
			
			//float angle = vectToAngle(differenceVector);
		
			moveVector = moveVector + differenceVector;
		}
		point moveVectorAccel = moveVector * acceleration;
		
		point gravAccel = { 0,0 };
		if (blackHole != nullptr) {
			gravAccel = blackHole->getAccelerationVector(this);
		}
		prevPosition = position;
		point totalAccelerationPositionDelta = (gravAccel + moveVectorAccel )* ((float)pow(timeElapsed, 2) / 2);
		//position = position + gravAccel * ((float)pow(timeElapsed, 2) / 2) + normalizeVector(normalizeVector(displacementVector) + normalizeVector(moveVector)) * ((float)pow(counter, 2) * acceleration / 2 + speed * timeElapsed);
		position = position + totalAccelerationPositionDelta+ normalizeVector(displacementVector)*speed* timeElapsed;

		if (counter == 2 || speed < FP_0) {
			position = fatherSpeed * timeElapsed + position;
		}
		point difference = position - prevPosition;
		float mag = magnitude(difference);
		//currentSpeed = mag / timeElapsed;
		displacementVector = difference * (1 / timeElapsed);
		rotation = vectToAngle(displacementVector);
		rotationDeg = rotation * 180.f / static_cast<float>(M_PI) + 90;
		getCollisionBox();
		if (counter > lifetime) {
			collision = true;
			collisionCoords = avgPosition;
			return true;
		}
		if (father == spaceships->at(0)) {
			//moveVector = displacementVector;
			for (auto s : *spaceships) {
				sf::Sprite sSprite = s->getSprite();
				
				if (s != father) {
				
					if (Collision::PixelPerfectTest(sprite, sSprite)) {//if collision
						// make the ship take damage
						s->takeDamage(damage);
						if (s->getHealth() <= 0) {
							father->increaseScore(s->getPointsValue());
						}

						collision = true;
						
						collisionCoords = avgPosition;
						
						return true;
						
					}
					else {
						
					}
				}
			}
		}
		else {
			auto s = spaceships->at(0);
			sf::Sprite sSprite = s->getSprite();
			
			if (s != father) {
				
				if (Collision::PixelPerfectTest(sprite, sSprite)) {//if collision
					// make the ship take damage
					s->takeDamage(damage);
					if (s->getHealth() <= 0) {
						father->increaseScore(s->getPointsValue());
					}
					// create explosion at point of intersection given priority, which comes from trajectory
					collision = true;
					collisionCoords = avgPosition;
					return true;
				}
				else {
				}
			}
		}
		
		moveClock.restart();
		moveSprite();
		return !inRange(position, windowSize);
	}
	else {
		return false;
	}
	
}