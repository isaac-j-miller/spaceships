#include "Mini.h"
#include "Carrier.h"
sf::Texture Mini::texture;
sf::Texture Mini::shieldTexture;
bool Mini::Init(const std::string& ImageFile) {
	shieldTexture.loadFromFile(shieldImageFileName);
	return texture.loadFromFile(ImageFile);
}
void Mini::setImage() {
	sprite.setTexture(texture);
	shieldSprite.setTexture(shieldTexture);
	shieldSprite.setColor(sf::Color(255, 255, 255, 100));

	float xScale = 1.1 * width / texture.getSize().x;
	float yScale = 1.1 * height / texture.getSize().y;

	sprite.setScale(sf::Vector2f(xScale, yScale));
	//do the same for shield
	xScale = 1.3 * width / shieldTexture.getSize().x;
	yScale = 1.3 * height / shieldTexture.getSize().y;
	shieldSprite.setOrigin(sf::Vector2f(shieldSprite.getTexture()->getSize().x * 0.5, shieldSprite.getTexture()->getSize().y * 0.5));
	shieldSprite.setScale(sf::Vector2f(xScale, yScale));

	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setPosition(avgPosition.x, avgPosition.y);
	sprite.setRotation(180 * rotation);
	shieldSprite.setPosition(avgPosition.x, avgPosition.y);
	shieldSprite.setRotation(180 * rotation);
}
Mini::Mini(point initPos, double initRotation, int d, Carrier* papa):
EnemySpaceship(initPos, initRotation,d){
	//std::cout << "mini init" << std::endl;
	father = papa;
	position = initPos;
	level = 0;
	width = 20;
	height = 20;
	health = 50;
	bulletPeriod = 15;
	speed = 2.8;
	pointValue = 400;

	tempTorpedoOrigin = { 0, 0 };
	tempBulletOrigin = { 0, 0 };
	setImage();
}
Mini::~Mini() {

}
void Mini::updateCollisionBox() {
	Spaceship::updateCollisionBox();
}
void Mini::fireBullet() {
	Spaceship::fireBullet();
}
void Mini::fireTorpedo() {
	fireBullet();
}
bool Mini::isAlive() { 
	bool a = Spaceship::isAlive(); 
	//remove self from list if 
	if (!a && fatherAlive) {
		for (auto it = father->children.begin(); it != father->children.end();) {
			if ((*it) == this) {
				it = father->children.erase(it);
			}
			else {
				++it;
			}
		}
	}
	
	return a;
}