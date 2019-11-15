#include "EnemyFighter.h"
EnemyFighter::EnemyFighter(point initPos, double initRotation, int d, int s) :
	Spaceship(initPos, initRotation),
	Fighter(initPos,initRotation),
	EnemySpaceship(initPos, initRotation, d, s) {
	movePeriod -= 5;
	setImage();
	//std::cout << "enemy fighter init at " << initPos << std::endl;
}
EnemyFighter::~EnemyFighter() {

}

void EnemyFighter::fireBullet() {
	Fighter::fireBullet();
}
void EnemyFighter::fireTorpedo() {
	Fighter::fireTorpedo();
}
void EnemyFighter::layMine() {
	Spaceship::layMine();
}
void EnemyFighter::updateCollisionBox() {
	Fighter::updateCollisionBox();
}

