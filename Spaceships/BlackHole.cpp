#include "BlackHole.h"
#include "Spaceship.h"

sf::Image BlackHole::image = sf::Image();
sf::Texture BlackHole::texture = sf::Texture();
bool BlackHole::Init(const std::string& ImageFile) {
	//return (image.loadFromFile(ImageFile) &&
	//	texture.loadFromImage(image));
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
	avgPosition = averagePosition(collisionBox);

	setImage();
	
	std::cout << "black hole init at" << avgPosition << ", size:" << size << std::endl;
}
void BlackHole::setImage() {
	sprite.setTexture(texture);
	float xScale = (float)size / texture.getSize().x;
	float yScale = (float)size / texture.getSize().y;
	//std::cout <<"t size: " << texture.getSize().x <<"sprite scale: (" << xScale << ", " << yScale << ")" << std::endl;
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setPosition(avgPosition.x, avgPosition.y);
	//std::cout << "sprite pos: (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ")" << std::endl;
}

void BlackHole::eatSpaceships() {
	auto cBox = getSprite().getGlobalBounds();
	for (auto a : *spaceships) {
		if (cBox.intersects(a->getSprite().getGlobalBounds())) {
			a->health = 0;
			
			size += a->getPointsValue() * growthConstant;
			pointsAccumulated += a->getPointsValue();
			std::cout << "spaceship eaten, size:" << size <<std::endl;

		}
	}
	updateSize();
}
void BlackHole::eatProjectiles() {
	auto cBox = getSprite().getGlobalBounds();
	for (auto a : *projectiles) {
		if (cBox.intersects(a->getSprite().getGlobalBounds())) {
			a->collision = true;
			a->eaten = true;
			std::cout << "projectile eaten" << std::endl;
		}
	}
	updateSize();
}
void BlackHole::updateSize() {
	width = size;
	height = size;
	float xScale = (float)size / texture.getSize().x;
	float yScale = (float)size / texture.getSize().y;
	//std::cout <<"t size: " << texture.getSize().x <<"sprite scale: (" << xScale << ", " << yScale << ")" << std::endl;
	sprite.setScale(sf::Vector2f(xScale, yScale));
}
point BlackHole::getAccelerationVector(Moveable* m) {
	point difference = m->getAvgPosition() - avgPosition;
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