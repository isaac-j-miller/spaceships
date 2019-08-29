#include "Bullet.h"
#include <iostream>
sf::Image Bullet::image = sf::Image();
sf::Texture Bullet::texture = sf::Texture();
bool Bullet::Init(const std::string& FileName) {
	return (image.loadFromFile(FileName) &&
			texture.loadFromImage(image));
}
Bullet::Bullet(point pos, point traj, int dmg, Spaceship* f) :
	Projectile(pos, traj, dmg, f) {
	speed = 4; //in pixels per frame
	explosionDuration = 20;//frames
    explosionSize = 10;
	width = 8;
	height = 16;
	baseTransform = { {-width / 2,-height / 2},{-height / 2,height / 2},{width / 2,height / 2},{width / 2,-height / 2} };
	maxDimension = std::max(width, height);
	setImage();
}
void Bullet::setImage() {
	sprite.setTexture(texture);
	sprite.setRotation(getRotation());
	float xScale = 1.1 * width / image.getSize().x;
	float yScale = 1.1 * height / image.getSize().y;
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setPosition(avgPosition.x, avgPosition.y);
	
}
