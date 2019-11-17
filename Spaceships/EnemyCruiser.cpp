#include "EnemyCruiser.h"
EnemyCruiser::EnemyCruiser(point initPos, double initRotation, int d, int s) :
	Spaceship(initPos, initRotation),
	Cruiser(initPos, initRotation),
	EnemySpaceship(initPos, initRotation, d,s) {
	movePeriod -= 2;
	setImage();

	getSpawnExplosion();
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
void EnemyCruiser::layMine() {
	Spaceship::layMine();
}
void EnemyCruiser::updateCollisionBox() {
	Cruiser::updateCollisionBox();
}

