#include "EnemyCruiser.h"
EnemyCruiser::EnemyCruiser(point initPos, double initRotation, int d) :
	Spaceship(initPos, initRotation),
	Cruiser(initPos, initRotation),
	EnemySpaceship(initPos, initRotation, d) {
	setImage();
	//std::cout << "enemy cruiser init at " << initPos << std::endl;
}
EnemyCruiser::~EnemyCruiser() {

}

void EnemyCruiser::fireBullet() {
	Cruiser::fireBullet();
}
void EnemyCruiser::fireTorpedo() {
	Cruiser::fireTorpedo();
}
void EnemyCruiser::updateCollisionBox() {
	Cruiser::updateCollisionBox();
}

