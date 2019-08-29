#include "Upgrade.h"
#include "SpaceshipFactory.h"
sf::Texture Upgrade::upgradeTexture;

Upgrade::Upgrade(point pos):PowerUp(pos,-1,0) {
	setImage();
	explosionString = "Level Up!";
}
Upgrade::~Upgrade() {

}
void Upgrade::Init(const std::string& fileName) {
	upgradeTexture.loadFromFile(fileName);
}
void Upgrade::setImage() {
	sprite.setTexture(upgradeTexture);
	float xScale = size / upgradeTexture.getSize().x;
	float yScale = size / upgradeTexture.getSize().y;
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x * 0.5, sprite.getTexture()->getSize().y * 0.5));
	sprite.setPosition(avgPosition.x, avgPosition.y);
}
void Upgrade::upgradeSpaceship(Spaceship* s) {
	//upgrade spaceship to new level
	//std::cout << "Upgrading Spaceship!" << std::endl;
	s = SpaceshipFactory::upgradePlayer(s);
}
bool Upgrade::isUpgrade() {
	return true;
}
bool Upgrade::isActive(Spaceship* player) {
	if (PowerUp::isActive(player)) {
		upgradeSpaceship(player);
		return true;
	}
	else {
		return false;
	}
}
