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
	//point avg = averagePosition(collisionBox);
	point difference;
	updateCollisionBox();
	//okey dokey let's figure out how to not get stuck
	if (time > movePeriod) {
		//std::cout << "calculating projectile vectors" << std::endl;
		//first, move away from other close spaceships & windowbounds. If no close spaceships and not on course for edge of map, then dodge bullets
		bool tooClose = false;
		//first, check window bounds
		displacementVector = getDisplacementVector();
		// disabling window bound part because adding wrapping
		/*
		static int threshold = 20;
		if (avgPosition.x < threshold && displacementVector.x <=0) { // if close to left side and moving left
			moveVector.x = 1;
			tooClose = true;
		}
		if (windowBounds.bottomRight.x -avgPosition.x < threshold && displacementVector.x >= 0) { // if close to right side and moving right
			moveVector.x = -1;
			tooClose = true;
		}
		if (avgPosition.y < threshold && displacementVector.y <= 0) { // if close to top and moving up
			moveVector.y = 1;
			tooClose = true;
		}
		if (windowBounds.bottomRight.y - avgPosition.y < threshold && displacementVector.y >= 0) { // if close to bottom and moving down
			moveVector.y = -1;
			tooClose = true;
		}
		*/
		box inflated = inflate(collisionBox, 2);
		
		//then, check spaceships
		for (auto s : *spaceships) {//iterate through spaceships and find those which overlap
			point sAvgPos = s->getAvgPosition();
			if (s != this && pointDistance(avgPosition, sAvgPos) < 2 * (getMaxDimension() + s->getMaxDimension())) {
				// check if overlap
				traj = s->getDisplacementVector();
				projectilePoint = sAvgPos;
				tempRay = { projectilePoint, traj * 10E4 + projectilePoint };
				if (lineIntersectBox(tempRay, inflated)) {
					rays.push_back(tempRay);
				}
				//tooClose = true;
			}
		}
		
		//if (!tooClose) {
			
			//then dodge bullets
			for (auto r : *projectiles) {//iterate through projectiles and find those that intersect this
				if (r->getFather() != this && pointDistance(avgPosition, r->getAvgPosition()) < 2 * (getMaxDimension() + r->getMaxDimension())) {
					traj = r->getTrajectory();
					projectilePoint = r->getAvgPosition();
					tempRay = { projectilePoint, traj * 10E4 + projectilePoint };
					if (lineIntersectBox(tempRay, inflated)) {
						rays.push_back(tempRay);
					}
				}
			}
			//std::cout << "rays: " << rays.size() << std::endl;
			//now rays contains all the rays that intersect the collisionBox
			
		//}
		for (auto r : rays) { //iterate over rays that intersect the collisionBox
			collisionPoint = collisionPoint + rayBoxIntersection(r, inflated);
		}
		if (rays.size() > 0) {
			collisionPoint = collisionPoint * (1.f / (rays.size())); //get average collision point
			difference = avgPosition - collisionPoint;
			//std::cout <<"num rays: "<<rays.size()<< "; collision point: " << collisionPoint << "; avg = " << avg << "; vector = " << moveVector << std::endl;
			if (difference.x != 0 || difference.y != 0) {
				moveVector = normalizeVector(difference);
			}
		}
	}
	return moveVector;
}
void EnemySpaceship::specialMove(){
	bool atk = true;
	if (player != nullptr && player->isAlive()) {	
		bool rotated = false;
		point target = player->getAvgPosition() + player->getDisplacementVector()*moveClock.getTime();
		//std::cout << "player"
		//std::cout << target << std::endl;
		static point randVect = { 0,0 };
		
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
		//now we have the rotation
		
		
		move(getMoveVector() * moveMod);
		rotated = setRotation(angle);
		
		if (rand() % 1000 < difficulty) { // determine whether or not to fire
			//disabling friendly fire prevention because friendly fire has been disabled
			/*
			//ADD A THING THAT KEEPS THE SPACESHIP FROM FIRING AT A FRIENDLY
			line_segment tempRay;
			for (auto s : *enemySpaceships) {//iterate through other enemy spaceships to avoid friendly fire
				if (s != this && pointDistance(position, s->getPosition()) < 2 * (getMaxDimension() + s->getMaxDimension())) {
					tempRay = { position, getTrajectory() * 10E4 + position };
					if (lineIntersectBox(tempRay, s->getCollisionBox())) {
						//std::cout << "on collision course. cancelling fire" << std::endl;
						atk = false;
					}
				}
			}
			
			if (atk) {
				attack();
			}
			*/
			attack();
		}
	}
	//else {
	//	health = 0;
	//}
}
void EnemySpaceship::attack() {// update 
	updateCollisionBox();
	bool br = bulletReady();
	bool tr = torpedoReady();
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
bool EnemySpaceship::isAlive() { return Spaceship::isAlive(); }