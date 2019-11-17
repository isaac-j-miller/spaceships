#include "Burst.h"


Burst::Burst(float framesDuration, float size, point centerPosition) {
	duration = framesDuration;
	size = size;
	avgPosition = centerPosition;
}
bool Burst::isActive() {
	//check timer 
	return(clock.getTime() < duration ? true : false);
}
Burst::Burst() {
	clock.restart();
}
sf::Sprite Burst::getSprite() {
	unsigned int cTime = clock.getTime();
	float progress = ((float)cTime) / duration;
	if (progress > .5) {
		progress = 1 - progress;
	}
	//progress += 0.5;
	int alpha = 255 * 2 * (progress);
	//std::cout << "alpha=" << alpha << std::endl;
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	return sprite;
}