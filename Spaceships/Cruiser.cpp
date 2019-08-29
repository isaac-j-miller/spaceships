#include "Cruiser.h"
sf::Texture Cruiser::texture;
sf::Texture Cruiser::shieldTexture;
Cruiser::Cruiser(point initPos, double initRotation) :
	Spaceship(initPos, initRotation) {
	level = 2;
	width = 40;
	height = 50;
	health = 250;
	torpedoDamage = 30;
	bulletPeriod = 18;
	speed = 2.5;
	pointValue = 4000;
	
	tempTorpedoOrigin = { -.5, 0 };
	tempBulletOrigin = { -.3, 0 };
	tempTorpedoOrigin2 = { .5, 0 };
	tempBulletOrigin2 = { .3, 0 };
	
	setDims();
	setImage();
}
Cruiser::~Cruiser() {

}
bool Cruiser::Init(const std::string& ImageFile) {
	//imageFile = ImageFile;
	shieldTexture.loadFromFile(shieldImageFileName);
	return texture.loadFromFile(ImageFile);
}
void Cruiser::setImage() {

	//texture.loadFromFile(imageFile);
	sprite.setTexture(texture);
	shieldSprite.setTexture(shieldTexture);
	shieldSprite.setColor(sf::Color(255, 255, 255, 100));

	float xScale = 1.1 * width / texture.getSize().x;
	float yScale = 1.1 * height / texture.getSize().y;

	sprite.setScale(sf::Vector2f(xScale, yScale));
	//do the same for shield
	xScale = 1.4 * width / shieldTexture.getSize().x;
	yScale = 1.4 * height / shieldTexture.getSize().y;
	shieldSprite.setOrigin(sf::Vector2f(shieldSprite.getTexture()->getSize().x * 0.5, shieldSprite.getTexture()->getSize().y * 0.5));
	shieldSprite.setScale(sf::Vector2f(xScale, yScale));

	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setPosition(avgPosition.x, avgPosition.y);
	sprite.setRotation(180 * rotation);
	shieldSprite.setPosition(avgPosition.x, avgPosition.y);
	shieldSprite.setRotation(180 * rotation);
}
void Cruiser::fireBullet() {
	bulletClock.restart();
	projectiles->push_back(new Bullet(bulletOrigin, getTrajectory(), bulletDamage, this));
	projectiles->push_back(new Bullet(bulletOrigin2, getTrajectory(), bulletDamage, this));
}
void Cruiser::fireTorpedo() {
	torpedoClock.restart();
	projectiles->push_back(new Torpedo(torpedoOrigin, getTrajectory(), torpedoDamage, this));
	projectiles->push_back(new Torpedo(torpedoOrigin2, getTrajectory(), torpedoDamage, this));
}
void Cruiser::updateCollisionBox() { // fix
	box pbox = rotate(baseTransform, rotation * M_PI);
	pbox = pbox + position;
	collisionBox = pbox;
	avgPosition = averagePosition(collisionBox);
	
	point wh = { width, height };
	torpedoOrigin = avgPosition + rotate(wh * tempTorpedoOrigin, rotation * M_PI);
	bulletOrigin = avgPosition + rotate(wh * tempBulletOrigin, rotation * M_PI);
	torpedoOrigin2 = avgPosition + rotate(wh * tempTorpedoOrigin2, rotation * M_PI);
	bulletOrigin2 = avgPosition + rotate(wh * tempBulletOrigin2, rotation * M_PI);
	//std::cout << "torpedo origins: " << torpedoOrigin << ", " << torpedoOrigin2 << std::endl;
	//std::cout << "bullet origins: " << bulletOrigin << ", " << bulletOrigin2 << std::endl;
}
bool Cruiser::isAlive() { return Spaceship::isAlive(); }