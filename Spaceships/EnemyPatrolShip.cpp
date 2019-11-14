#include "EnemyPatrolShip.h"
#include "GuidedMissile.h"
EnemyPatrolShip::EnemyPatrolShip(point initPos, double initRotation, int d): 
	Spaceship(initPos,initRotation),
	EnemySpaceship(initPos, initRotation, d),
	PatrolShip(initPos,initRotation)
	{
	setImage();
	//std::cout << "enemy patrol init at " << initPos << std::endl;

}
EnemyPatrolShip::~EnemyPatrolShip() {

}
void EnemyPatrolShip::fireBullet() {
	PatrolShip::fireBullet();
}
void EnemyPatrolShip::fireTorpedo() {
	PatrolShip::fireTorpedo();
	
}
void EnemyPatrolShip::layMine() {
	Spaceship::layMine();
}
void EnemyPatrolShip::updateCollisionBox() {
	PatrolShip::updateCollisionBox();
}

