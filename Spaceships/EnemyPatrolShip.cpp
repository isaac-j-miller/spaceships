#include "EnemyPatrolShip.h"

EnemyPatrolShip::EnemyPatrolShip(point initPos, double initRotation, int d): 
	Spaceship(initPos,initRotation),
	PatrolShip(initPos,initRotation),
	EnemySpaceship(initPos,initRotation,d){
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
void EnemyPatrolShip::updateCollisionBox() {
	PatrolShip::updateCollisionBox();
}
bool EnemyPatrolShip::isAlive() { return Spaceship::isAlive(); }