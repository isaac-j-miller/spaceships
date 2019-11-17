#include "Explosion.h"
#include "Collision.h"
#include "Spaceship.h"
sf::Image Explosion::image = sf::Image();
sf::Texture Explosion::texture = sf::Texture();


Explosion::Explosion(float frameDuration, float s, point centerPosition, int dmg):
	Burst(frameDuration, s, centerPosition)
{
	clock.restart();
	damagePerFrame = dmg;
	duration = frameDuration;
	size = s;
	point trans = { s/2,s/2 };
	position = centerPosition - trans;
	setImage();
	//do sprite stuff
}

sf::Sprite Explosion::getSprite() {
	unsigned int cTime = clock.getTime();
	float progress = ((float)cTime) / duration;
	if (progress > .5) {
		progress = 1 - progress;
	}
	//progress += 0.5;
	int alpha = 255 *2* (progress);
	//std::cout << "alpha=" << alpha << std::endl;
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	if (damagePerFrame&&!cTime%5) {
		for (auto s : *spaceships) {
			if (Collision::PixelPerfectTest(sprite, s->getSprite())) {//if collision
				// make the ship take damage
				s->takeDamage(damagePerFrame);
			}
		}
	}
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
	sprite.setScale(sf::Vector2f(xScale, yScale));
	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
	sprite.setPosition(avgPosition.x, avgPosition.y);
}