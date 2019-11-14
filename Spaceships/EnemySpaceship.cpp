#include "EnemySpaceship.h"
#include <random>
Spaceship* EnemySpaceship::player = nullptr;
EnemySpaceship::EnemySpaceship(point initPos, double initRotation,int d) :
	Spaceship(initPos, initRotation) {
	sprite.setColor(sf::Color(255,150,150,255));
	srand(initRotation);
	difficulty = d;
	enemy = true;
	score += difficulty * 100;
	moveTimer.restart();
	
	oldRotation = 1;
	point tempPoint = { 0.001, 0.001 };
	prevPosition = initPos - tempPoint;
	updateCollisionBox();
	
}
EnemySpaceship::~EnemySpaceship() {

}
void EnemySpaceship::Init(Spaceship* p) {
	player = p;
}

point EnemySpaceship::getMoveVector() {
	long long unsigned int time = moveTimer.getTime();
	std::vector<line_segment> rays = {};
	point projectilePoint;
	point traj;
	line_segment tempRay;
	point collisionPoint;
	point difference;
	updateCollisionBox();
	
	box inflated = inflate(collisionBox, 4);
		
	for (auto r : *projectiles) {//iterate through projectiles and find those that intersect this
		if (r->getFather() != this && pointDistance(avgPosition, r->getAvgPosition()) < 4 * (getMaxDimension() + r->getMaxDimension())) {
			traj = r->getTrajectory();
			projectilePoint = r->getAvgPosition();
			tempRay = { projectilePoint, traj * 10E4 + projectilePoint };
			if (lineIntersectBox(tempRay, inflated)) {
				rays.push_back(tempRay);
			}
		}
	}
	if (blackHole != nullptr) {
		traj = avgPosition - blackHole->getPosition();
		if (magnitude(traj) < 4 * blackHole->getRadius()) {
			traj = normalizeVector(traj)*-1;
			projectilePoint = blackHole->getPosition();
			tempRay = { avgPosition, traj * 10E4 + avgPosition };
			if (lineIntersectBox(tempRay, inflated)) {
				for (int i = 0; i < 5; i++) {
					rays.push_back(tempRay);
				}
			}
		}
	}
			
	//now rays contains all the rays that intersect the collisionBox
			
	for (auto r : rays) { //iterate over rays that intersect the collisionBox
		collisionPoint = collisionPoint + rayBoxIntersection(r, inflated);
	}
	if (rays.size() > 0) {
		collisionPoint = collisionPoint * (1.f / (rays.size())); //get average collision point
		difference = avgPosition - collisionPoint;
		//std::cout <<"num rays: "<<rays.size()<< "; collision point: " << collisionPoint << "; vector = " << moveVector << std::endl;
		if (difference.x != 0 || difference.y != 0) {
			moveVector = normalizeVector(difference);
		}
	}

	//else {
	//	moveVector = { 0,0 };
	//}
	//std::cout << moveVector<<","<<position<< std::endl;
	return moveVector;
}
void EnemySpaceship::specialMove(){
	bool atk = true;
	if (player != nullptr && player->isActive()) {	
		bool rotated = false;
		point target = player->getAvgPosition() + player->getDisplacementVector();
		float angle = rotation;
		if (target.x != avgPosition.x) { // if target is not vertical to position
			if (target.x > avgPosition.x) {
				angle = atan((avgPosition.y - target.y) / (avgPosition.x - target.x)) / M_PI - .5; //might have to fix this
			}
			else {
				angle = atan((avgPosition.y - target.y) / (avgPosition.x - target.x)) / M_PI + .5; //might have to fix this
			}
		}
		else if (target.x > avgPosition.x) { // if target is directly above position
			angle = 1;
		}
		else {
			angle = 0;
		}
		move(getMoveVector());
		rotated = setRotation(angle);
		if (rand() % 1000 < difficulty) { // determine whether or not to fire
			attack();
		}
	}
}
void EnemySpaceship::attack() {// update 
	updateCollisionBox();
	bool br = bulletReady();
	bool tr = torpedoReady();
	bool mr = mineReady();
	int code = 0;
	if (br && !tr) {
		code = 1;
	}
	else if (!br && tr) {
		code = 0;
	}
	else if (br && tr) {
		code =(rand() % 2);
	}
	switch (code) {
		case 0:
			fireTorpedo();
		default:
			fireBullet();
	}
	if (getLevel() >= 1 && mr) {
		layMine();
	}
} 

void EnemySpaceship::updateCollisionBox() {
	Spaceship::updateCollisionBox();
}
void EnemySpaceship::fireBullet() {
	Spaceship::fireBullet();
}
void EnemySpaceship::fireTorpedo() {
	Spaceship::fireTorpedo();
}
void EnemySpaceship::layMine() {
	Spaceship::layMine();
}
