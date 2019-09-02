#include "PatrolShip.h"
sf::Texture PatrolShip::texture;
sf::Texture PatrolShip::shieldTexture;
PatrolShip::PatrolShip(point initPos, double initRotation):
	Spaceship(initPos,initRotation) {
	level = 0;
	width = 30;
	height = 30;
	speed = getFreq(4);
	point tempTorpedoOrigin = { -.5,0 };
	point tempBulletOrigin = { .5,0 };
	setDims(width, height);
	setImage();
}
PatrolShip::~PatrolShip() {

}
bool PatrolShip::Init(const std::string& ImageFile) {
	shieldTexture.loadFromFile(shieldImageFileName);
	return texture.loadFromFile(ImageFile);
}
void PatrolShip::setImage() {
	//texture.loadFromFile(imageFile);
	sprite.setTexture(texture);
	shieldSprite.setTexture(shieldTexture);
	shieldSprite.setColor(sf::Color(255, 255, 255, 100));

	float xScale = 1.1 * width / texture.getSize().x;
	float yScale = 1.1 * height / texture.getSize().y;

	sprite.setScale(xScale, yScale);
	//do the same for shield
	xScale = 1.3 * width / shieldTexture.getSize().x;
	yScale = 1.3 * height / shieldTexture.getSize().y;
	shieldSprite.setOrigin(shieldSprite.getTexture()->getSize().x * 0.5, shieldSprite.getTexture()->getSize().y * 0.5);
	shieldSprite.setScale(xScale, yScale);


	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setPosition(avgPosition.x, avgPosition.y);
	sprite.setRotation(180 * rotation);
	shieldSprite.setPosition(avgPosition.x, avgPosition.y);
	shieldSprite.setRotation(180 * rotation);
}
void PatrolShip::fireBullet() {
	Spaceship::fireBullet();
}
void PatrolShip::fireTorpedo() {
	Spaceship::fireTorpedo();
}
void PatrolShip::updateCollisionBox() {
	Spaceship::updateCollisionBox();
}
