#include "SpaceThing.h"
#include "EnemySpaceship.h"
#include "Bullet.h"
#include "Torpedo.h"
sf::RenderWindow SpaceThing::window;
box SpaceThing::windowBounds;
std::vector<Spaceship*> SpaceThing::spaceships;
std::vector<EnemySpaceship*> SpaceThing::enemySpaceships;
std::vector<Projectile*> SpaceThing::projectiles;
std::vector<Explosion*> SpaceThing::explosions;
sf::Image SpaceThing::background;
sf::Texture SpaceThing::backgroundTexture;
sf::Sprite SpaceThing::bSprite;
sf::Font SpaceThing::font;
sf::Text SpaceThing::healthReadout;
sf::Clock SpaceThing::masterClock;
Spaceship* SpaceThing::player = nullptr;
long long unsigned int SpaceThing::frame = 0;
float SpaceThing::framerate = 1024; //fps
float SpaceThing::framePeriodMicroSeconds = (1 / framerate) * 10E6;


void SpaceThing::Inititiate(point size, int numEnemies, int difficulty) {
	Spaceship::Init("spaceship.png");
	Bullet::Init("bullet.png");
	Torpedo::Init("torpedo.png");
	Explosion::Init("explosion.png");
	//
	font.loadFromFile("pixel_font.ttf");
	healthReadout.setFont(font);
	healthReadout.setPosition(0, 0);
	healthReadout.setCharacterSize(30);
	healthReadout.setFillColor(sf::Color::Red);
	healthReadout.setStyle(sf::Text::Bold);
	window.create(sf::VideoMode(size.x, size.y), "Spaceships");
	background.loadFromFile("background1.png");
	backgroundTexture.loadFromImage(background);
	bSprite.setTexture(backgroundTexture);
	point scale = { size.x / backgroundTexture.getSize().x,size.y / backgroundTexture.getSize().y };
	bSprite.setScale(scale.x, scale.y);

	spaceships.push_back(new Spaceship({ size.x / 2,size.y - 50 }, 1));//instantiate player
	player = spaceships[0];
	EnemySpaceship::Init(player); // initialize enemySpaceships with player as the target
	point randpos;
	bool c = false; //used to make sure RNG doesn't place 2 spaceships in the same spot
	
	for (float i = 0; i < numEnemies; i++) {
		srand(masterClock.getElapsedTime().asMicroseconds());
		do {
			randpos = { ((rand() % 80 + 10) / 100.f) * (size.x),((rand() % 80 + 10) / 100.f) * (size.y) };
			for (auto s : spaceships) {
				if (pointDistance(s->getPosition(), randpos) < 2.1 * s->getMaxDimension()) {
					c = true;
				}
				else {
					c = false;
				}
			}
		} while (c);
		EnemySpaceship* e = new EnemySpaceship(randpos, 0., difficulty);
		spaceships.push_back(e);//instantiate enemies
		enemySpaceships.push_back(e);
	}
	std::cout << "windowbox: " << std::endl;
	const box windowBox = { { 0,0 } ,{0,size.y},{size.x,size.y},{size.x,0} };
	std::cout << "windowbox: " << windowBox << std::endl;
	FrameClock::Init(frame);
	window.display(); //show the window
}
void SpaceThing::draw() {
	window.clear();
	window.draw(bSprite);//draw the background
	int code;
	for (auto it = projectiles.begin(); it != projectiles.end();) {//iterate through projectiles and draw the active ones and explode/disappear the dead/OOB ones
		if (!(*it)->move()) {
			window.draw((*it)->getSprite());
			++it;
		}
		else {
			if ((*it)->getCollision()) {
				explosions.push_back((*it)->getExplosion());
			}
			delete* it;
			std::cout << "erasing projectile at " << it[0] << std::endl;
			it = projectiles.erase(it);
		}
	}
	for (auto it = enemySpaceships.begin(); it != enemySpaceships.end();) { //iterate through enemy spaceships and remove the dead ones (doesn't delete; next block does that) and AI the alive ones
		if ((*it)->isAlive()) {
			code = (*it)->specialMove();
			if (code != -1) {
				projectiles.push_back((*it)->attack(code));
			}
			++it;
		}
		else {
			std::cout << "erasing enemy spaceship at " << it[0] << std::endl;
			it = enemySpaceships.erase(it);
		}
	}
	for (auto it = spaceships.begin(); it != spaceships.end();) { //iterate through all spaceships and draw the alive ones and explode the dead ones
		if ((*it)->isAlive()) {
			window.draw((*it)->getSprite());
			++it;
		}
		else {
			explosions.push_back((*it)->explode());
			delete* it;
			std::cout << "erasing spaceship at " << it[0] << std::endl;
			it = spaceships.erase(it);
		}
	}

	for (auto it = explosions.begin(); it != explosions.end();) { //draw/disappear all explosions
		if ((*it)->isActive()) {
			//std::cout << "drawing explosion" << std::endl;
			window.draw((*it)->getSprite());
			++it;
		}
		else {
			delete* it;
			//std::cout << "erasing explosion at " << it[0] << std::endl;
			it = explosions.erase(it);
		}
	}
	if (player->isAlive()) {
		healthReadout.setString("Player health: " + std::to_string(player->getHealth()));
	}
	else {
		healthReadout.setString("Player health: 0");
	}
	window.draw(healthReadout);
	
}
void SpaceThing::getUserInput() {
	point moveVector = { 0,0 };
	float rotation = 0;
	bool move = false;
	bool rotate = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		moveVector.x = -1;
		player->move(moveVector);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		moveVector.x = 1;
		player->move(moveVector);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		moveVector.y = -1;
		player->move(moveVector);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		moveVector.y = 1;
		player->move(moveVector);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		rotation -= 1;
		player->incrementRotation(rotation);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		rotation += 1;
		player->incrementRotation(rotation);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket)) {
		if (player->bulletReady()) {
			projectiles.push_back(player->fireBullet());
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket)) {
		if (player->torpedoReady()) {
			projectiles.push_back(player->fireTorpedo());
		}

	}
}

void SpaceThing::doFrame() {
	masterClock.restart();
	FrameClock::updateTime(frame);
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		//check keypresses
		getUserInput();
	}
	//make sure the frame doesn't draw too fast
	while (masterClock.getElapsedTime().asMicroseconds() < framePeriodMicroSeconds) {}
	//draw
	draw();
	window.display(); //show the window
	frame++;
	window.clear();
}

point SpaceThing::getPosition() {
	return position;
}
box SpaceThing::getCollisionBox() {
	return collisionBox;
}
point SpaceThing::getAvgPosition() {
	return avgPosition;
}
sf::Sprite SpaceThing::getSprite() {
	return sprite;
}
void SpaceThing::setImage() {

}
void SpaceThing::moveSprite() {

}
float SpaceThing::getMaxDimension() {
	return maxDimension;
}
float SpaceThing::getRadius() {
	return radius;
}
float SpaceThing::getRotation() {
	return rotation;
}