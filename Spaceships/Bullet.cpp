#include "Bullet.h"
#include <iostream>
sf::Image Bullet::image = sf::Image();
sf::Texture Bullet::texture = sf::Texture();
bool Bullet::Init(const std::string& FileName) {
	return (image.loadFromFile(FileName) &&
			texture.loadFromImage(image));
}
Bullet::~Bullet() {
	//std::cout << "del bullet" << std::endl;
}
Bullet::Bullet(point pos, point traj, int dmg, Spaceship* f) :
	Projectile(pos, traj, dmg, f) {
	//std::cout << "new bullet" << std::endl;
	speed = 10; //in pixels per frame
	explosionDuration = 10;//frames
    explosionSize = 10;
	width = 3;
	height = 6;
	setDims(width, height);
	setImage();
}
void Bullet::setImage() {
	sprite.setTexture(texture);
	sprite.setRotation(getRotation());
	float xScale = 1 * width / image.getSize().x;
	float yScale = 1 * height / image.getSize().y;
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setPosition(avgPosition.x, avgPosition.y);

}
