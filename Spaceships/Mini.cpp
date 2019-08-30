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
	bulletPeriod = 7;
	speed = 5.6;
	pointValue = 400;
	mini = true;
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
void Mini::move(point inputVector) {
	inputVector = normalizeVector(inputVector);
	updateCollisionBox();
	if (health > 0) {
		prevPosition = position;
		elapsedFrames = moveClock.getTime();
		position = position + inputVector * speed;
		if (inRange(position, windowBounds.bottomRight)) {
			//if wrapped
			position = getWrapped(position, windowBounds.bottomRight);
			prevPosition = position - inputVector * speed;
		}
		//disable bc wrapping
		/*
		//check if within bounds
		if (!boxWithin(collisionBox, windowBounds)) { // if the collisionbox is not fully within the bounds
			// check which direction the thing is going and figure out which edge it's closest to & block if moving closer to edge
			point direction = (avgPosition - averagePosition(windowBounds)) * inputVector;
			if (direction.x > 0 || direction.y > 0) { // moving in wrong direction
				position = prevPosition;
			}

		}
		*/
		//else {// if (boxWithin(collisionBox, inflate(windowBounds, .93))){ // if within bounds and 
			//iterate over other spaceships
			for (auto a : *spaceships) {
				if (a != this && a!=father) { // can pass through father
					point aPos = a->getAvgPosition();

					if (pointDistance(avgPosition, aPos) < (getMaxDimension() + a->getMaxDimension())) { // if spaceship is close
						box aBox = a->getCollisionBox();
						point direction = (avgPosition - aPos) * inputVector;
						if (boxOverlap(collisionBox, aBox)) {
							if (direction.x < 0 || direction.y < 0) { // moving in wrong direction
								position = prevPosition;
							}
							break;
						}
						else if (boxWithin(collisionBox, aBox)) {
							if (direction.x < 0 || direction.y < 0) { // moving in wrong direction
								position = prevPosition;
							}
							break;
						}
					}
				}
			}
		//}
	}
	//calculate displacement vector
	if (elapsedFrames != 0) {
		displacementVector = (position - prevPosition) * (1 / elapsedFrames);
	}
	moveSprite();
	moveClock.restart();
}