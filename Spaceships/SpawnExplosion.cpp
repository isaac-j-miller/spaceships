#include "SpawnExplosion.h"
#include "Collision.h"
sf::Image SpawnExplosion::image = sf::Image();
sf::Texture SpawnExplosion::texture = sf::Texture();


SpawnExplosion::SpawnExplosion(float s, point centerPosition){
	clock.restart();
	duration = 15;
	size = 0;
	initSize = s*8;
	//point trans = { s / 2,s / 2 };
	position = centerPosition;// - trans;
	setImage();
	//do sprite stuff
}

sf::Sprite SpawnExplosion::getSprite() {
	unsigned int cTime = clock.getTime();
	float progress = ((float)cTime) / duration;
	if (progress > .5) {
		progress = 1 - progress;
	}
	//progress += 0.5;
	//int alpha = 255 * 2 * (progress);
	size = initSize*progress;
	//std::cout << "alpha=" << alpha << std::endl;
	//sprite.setColor(sf::Color(255, 255, 255, alpha));
	float xScale = size / image.getSize().x;
	float yScale = size / image.getSize().y;
	sprite.setScale(sf::Vector2f(xScale, yScale));
	//std::cout << "spawnexplosion size: " << size << std::endl;
	return sprite;
}
bool SpawnExplosion::Init(const std::string& FileName) {
	return (image.loadFromFile(FileName) &&
		texture.loadFromImage(image));
}
void SpawnExplosion::setImage() {
	sprite.setTexture(texture);
	float xScale = size / image.getSize().x;
	float yScale = size / image.getSize().y;
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	sprite.setPosition(position.x, position.y);
}