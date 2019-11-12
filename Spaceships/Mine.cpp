#include "Mine.h"
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