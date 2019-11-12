#include "BlackHole.h"
#include "Spaceship.h"
#include "Collision.h"
#include "PowerUp.h"

sf::Image BlackHole::image = sf::Image();
sf::Texture BlackHole::texture = sf::Texture();
bool BlackHole::Init(const std::string& ImageFile) {
	//return (image.loadFromFile(ImageFile) &&
	//	texture.loadFromImage(image));
	//return Collision::CreateTextureAndBitmask(texture,	ImageFile);
	return (texture.loadFromFile(ImageFile));
}

BlackHole::BlackHole(point pos) {
	position = pos;
	size = initialSize;
	updateSize();
	setDims(width, height);
	blackHole = this;
	collisionBox = { 0,0,(float)size,(float)size };
	collisionBox = collisionBox + position;
	//avgPosition = averagePosition(collisionBox);

	setImage();
	
	//std::cout << "black hole init at" << avgPosition << ", size:" << size << std::endl;
}
void BlackHole::setImage() {
	sprite.setTexture(texture);
	float xScale = (float)size / texture.getSize().x;
	float yScale = (float)size / texture.getSize().y;
	//std::cout <<"t size: " << texture.getSize().x <<"sprite scale: (" << xScale << ", " << yScale << ")" << std::endl;
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setPosition(position.x, position.y);
	//std::cout << "sprite pos: (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ")" << std::endl;
}

void BlackHole::eatSpaceships() {
	//auto cBox = getSprite().getGlobalBounds();
	for (auto a : *spaceships) {
		if (Collision::PixelPerfectTest(sprite, a->getSprite())) {
		//if (cBox.intersects(a->getSprite().getGlobalBounds())) {
			a->health = 0;
			size += a->getPointsValue() * growthConstant/size;
			pointsAccumulated += a->getPointsValue();
			//std::cout << "spaceship eaten, size:" << size <<std::endl;

		}
	}
	updateSize();
}
void BlackHole::eatProjectiles() {
	//auto cBox = getSprite().getGlobalBounds();
	for (auto a : *projectiles) {
		if (Collision::PixelPerfectTest(sprite, a->getSprite())) {
			a->collision = true;
			a->eaten = true;
			pointsAccumulated += projectilePoints;
			size += projectilePoints * growthConstant / size;
			//std::cout << "projectile eaten" << std::endl;
		}
	}
	updateSize();
}
void BlackHole::eatPowerUps() {
	//auto cBox = getSprite().getGlobalBounds();
	for (auto a : *powerUps) {
		if (Collision::PixelPerfectTest(sprite, a->getSprite())) {
			a->collision = true;
			a->eaten = true;
			//std::cout << "powerup eaten" << std::endl;
		}
	}
	//updateSize();
}

unsigned long int BlackHole::getMass() {
	return pointsAccumulated;
}
void BlackHole::updateSize() {
	width = size;
	height = size;
	//std::cout << "blackhole size:" << size << std::endl;
	polarMomentOfInertia = M_PI * pow(size / 2, 2) / 2;
	rotationSpeed = angularMomentum / (polarMomentOfInertia);
	rotation = oldRotation + rotationSpeed;
	oldRotation = rotation;
	float xScale = (float)size / texture.getSize().x;
	float yScale = (float)size / texture.getSize().y;
	//std::cout <<"t size: " << texture.getSize().x <<"sprite scale: (" << xScale << ", " << yScale << ")" << std::endl;
	sprite.setRotation(rotation * 360);
	sprite.setScale(sf::Vector2f(xScale, yScale));
}
float BlackHole::getDistance(Moveable* m) {
	point difference = m->getAvgPosition() - position;
	return magnitude(difference);
}
float BlackHole::getAngle(Moveable* m) {
	point difference = position-m->getAvgPosition();
	return vectToAngle(difference);
}
point BlackHole::getAccelerationVector(Moveable* m) {
	point difference = m->getAvgPosition() - position;
	float distance = magnitude(difference);
	float factor = -gravitationalConstant * pointsAccumulated/ pow(distance, 2);
	point effect = normalizeVector(difference) * factor;
	//std::cout << "difference, distance: " << difference << ", " << distance << std::endl;
	return effect; //is the gravitational acceleration from black hole
}
sf::Sprite BlackHole::getSprite() {
	//sprite.setPosition(avgPosition.x, avgPosition.y);
	//std::cout <<"sprite pos: ("<< sprite.getPosition().x << ", "<< sprite.getPosition().y << ")" << std::endl;
	//std::cout << "sprite dims: (" << sprite.getGlobalBounds().width << ", " << sprite.getGlobalBounds().height << ")" << std::endl;
	return sprite;
}