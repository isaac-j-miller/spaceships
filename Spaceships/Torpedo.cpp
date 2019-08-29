#include "Torpedo.h"
#include "Explosion.h"

#include <iostream>
sf::Image Torpedo::image = sf::Image();
sf::Texture Torpedo::texture= sf::Texture();
bool Torpedo::Init(const std::string& FileName) {
	return (image.loadFromFile(FileName) &&
			texture.loadFromImage(image));
	
}
Torpedo::Torpedo(point pos, point traj, int dmg, Spaceship* f) :
	Projectile(pos, traj, dmg, f) {

	speed = 7; //in pixels per frame
	explosionDuration = 10;//frames
	explosionSize = 20;
	width = 10;
	height = 15;
	baseTransform = { {-width / 2,-height / 2},{-height / 2,height / 2},{width / 2,height / 2},{width / 2,-height / 2} };
	acceleration = 3E-2; // pixels/(frame^2)
	maxDimension = std::max(width, height);
	setImage();

}

void Torpedo::setImage() {
	sprite.setTexture(texture);
	sprite.setRotation(getRotation());
	float xScale = 1.1 * width / image.getSize().x;
	float yScale = 1.1 * height / image.getSize().y;
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setPosition(avgPosition.x, avgPosition.y);
	
}

