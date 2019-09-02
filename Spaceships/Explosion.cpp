#include "Explosion.h"
sf::Image Explosion::image = sf::Image();
sf::Texture Explosion::texture = sf::Texture();

Explosion::Explosion() {
	clock.restart();
}
Explosion::Explosion(float frameDuration, float s, point centerPosition) {
	clock.restart();
	duration = frameDuration;
	size = s;
	point trans = { s/2,s/2 };
	position = centerPosition - trans;
	setImage();
	//do sprite stuff
}
bool Explosion::isActive() {
	//check timer 
	return(clock.getTime() < duration ? true : false);
}
sf::Sprite Explosion::getSprite() {
	float cTime = clock.getTime();
	float progress = cTime / duration;
	if (progress > .5) {
		progress = 1 - progress;
	}
	//progress += 0.5;
	int alpha = 255 *2* (progress);
	//std::cout << "alpha=" << alpha << std::endl;
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	
	return sprite;
}
bool Explosion::Init(const std::string& FileName) {
	return (image.loadFromFile(FileName) &&
			texture.loadFromImage(image));
}
void Explosion::setImage() {
	sprite.setTexture(texture);
	float xScale = size / image.getSize().x;
	float yScale = size / image.getSize().y;
	sprite.setScale(xScale, yScale);
	sprite.setPosition(position.x-size/8, position.y-size/8);
}