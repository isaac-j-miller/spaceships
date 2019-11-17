#include "Mine.h"
#include "Spaceship.h"
#include "Collision.h"
sf::Image Mine::image = sf::Image();
sf::Texture Mine::texture = sf::Texture();
bool Mine::Init(const std::string& FileName) {
	return (image.loadFromFile(FileName) &&
		texture.loadFromImage(image));
}
Mine::~Mine() {
	//std::cout << "del Mine" << std::endl;
}
Mine::Mine(point pos, point traj, int dmg, Spaceship* f) :
	Projectile(pos, traj, dmg, f) {
	//std::cout << "new Mine" << std::endl;
	speed = 0; //in pixels per frame 
	explosionDuration = 40;//frames
	explosionSize = 50;
	explosionDamage = 2;
	width = 20;
	height = 20;
	setDims(width, height);
	setImage();
}
void Mine::setImage() {
	sprite.setTexture(texture);
	sprite.setRotation(getRotation());
	float xScale = 1 * width / image.getSize().x;
	float yScale = 1 * height / image.getSize().y;
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setPosition(avgPosition.x, avgPosition.y);

}
bool Mine::move() {
	counter++;
	unsigned long long int timeElapsed = moveClock.getTime();
	if (eaten) {
		return true;
	}
	if (timeElapsed != 0) {
		point gravAccel = { 0,0 };
		if (blackHole != nullptr) {
			gravAccel = blackHole->getAccelerationVector(this);
		}
		prevPosition = position;
		position = position + gravAccel * ((float)pow(timeElapsed, 2) / 2) + normalizeVector(displacementVector) * ((float)pow(counter, 2) * acceleration / 2 + speed * timeElapsed);

		if (counter == 2 || speed < FP_0) {
			position = fatherSpeed * timeElapsed + position;
		}
		point difference = position - prevPosition;
		float mag = magnitude(difference);
		//currentSpeed = mag / timeElapsed;
		displacementVector = difference * (1 / timeElapsed);
		rotation = vectToAngle(displacementVector);
		//std::cout << "rotation: " << vectToAngle(trajectory) << std::endl;
		rotationDeg = rotation * 180.f / static_cast<float>(M_PI) + 90;
		getCollisionBox();

		if (counter > lifetime) {
			collision = true;
			collisionCoords = avgPosition;
			return true;
		}
		for (auto p : *projectiles) {
			if (p != this) {
				sf::Sprite pSprite = p->getSprite();
				if (Collision::PixelPerfectTest(sprite, pSprite)) {//if collision
					p->collideWithMine();
					collideWithMine();
					//std::cout << "Collision at " << collisionCoords << std::endl;
					return true;
					//std::cout << std::endl << "done with this spaceship. Collision true" << std::endl;
				}
			}
		}
		if (father == spaceships->at(0)) {
			for (auto s : *spaceships) {
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

				if (Collision::PixelPerfectTest(sprite, sSprite)) {//if collision
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