#include <SFML/Graphics.hpp>
#include "Bullet.h"//includes projectile
#include "Torpedo.h"
#include "GuidedMissile.h"
#include "Mine.h"
#include "TextExplosion.h"
//#include "EnemySpaceship.h" // includes spaceship
#include "PowerUp.h"
#include "Upgrade.h"
#include "PowerUpFactory.h"
#include "SpaceshipFactory.h"
#include "EnemyPatrolShip.h"
#include "EnemyFighter.h"
#include "EnemyCruiser.h"
#include "Carrier.h"
#include "Mini.h"
#include "BlackHole.h"
#include "SpawnExplosion.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>



const int thresholds[] = { 10000, 25000, 50000 };
const int MAX_LEVEL = 2;
const int SPACESHIP_TYPES = MAX_LEVEL + 1;
const char* logFile = "scores.txt";
unsigned long long int frame = 0;
unsigned int highScore;
const float framerate = 512; //fps
float framePeriodMicroSeconds = (1 / framerate) * 10E6;
unsigned int prevScore = 0;
bool paused = false;
const point size = { 1280, 960 };
const box windowBox = { { 0,0 } ,{0,size.y},{size.x,size.y},{size.x,0} };
int count = 0;
sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Spaceships");
sf::Font font;
sf::Text healthReadout;
sf::Text score;
sf::Text highScoreText;
sf::Text pauseText;
sf::Text endText;
sf::Text levelDisplay;
sf::Text warningDisplay;
sf::Text controlsText;
sf::Text playerStatus;
sf::Image background;
sf::Texture backgroundTexture;
sf::Sprite bSprite;
sf::Clock masterClock;
std::string shieldImageFileName;
std::vector<Spaceship*> spaceships;
std::vector<EnemySpaceship*> enemySpaceships;
std::vector<Projectile*> projectiles;
std::vector<Burst*> explosions;
std::vector<PowerUp*> powerUps;


void showBounds(Spaceship* s) {
	auto c = s->getCollisionBox();
	//std::cout << c << std::endl;
	auto r1 = sf::CircleShape(2);
	r1.setFillColor(sf::Color::Blue);
	r1.setPosition(c.topLeft.x, c.topLeft.y);
	window.draw(r1);

	auto r2 = sf::CircleShape(2);
	r2.setFillColor(sf::Color::Red);
	r2.setPosition(c.bottomLeft.x, c.bottomLeft.y);
	window.draw(r2);

	auto r3 = sf::CircleShape(2);
	r3.setFillColor(sf::Color::Green);
	r3.setPosition(c.bottomRight.x, c.bottomRight.y);
	window.draw(r3);

	auto r4 = sf::CircleShape(2);
	r4.setFillColor(sf::Color::Yellow);
	r4.setPosition(c.topRight.x, c.topRight.y);
	window.draw(r4);
}
void showBounds(PowerUp* s) {
	auto c = s->getCollisionBox();
	auto r1 = sf::CircleShape(2);
	r1.setFillColor(sf::Color::Blue);
	r1.setPosition(c.topLeft.x, c.topLeft.y);
	window.draw(r1);

	auto r2 = sf::CircleShape(2);
	r2.setFillColor(sf::Color::Red);
	r2.setPosition(c.bottomLeft.x, c.bottomLeft.y);
	window.draw(r2);

	auto r3 = sf::CircleShape(2);
	r3.setFillColor(sf::Color::Green);
	r3.setPosition(c.bottomRight.x, c.bottomRight.y);
	window.draw(r3);

	auto r4 = sf::CircleShape(2);
	r4.setFillColor(sf::Color::Yellow);
	r4.setPosition(c.topRight.x, c.topRight.y);
	window.draw(r4);
}
void showBounds(Projectile* s) {
	auto c = s->getCollisionBox();
	auto r1 = sf::CircleShape(2);
	r1.setFillColor(sf::Color::Blue);
	r1.setPosition(c.topLeft.x, c.topLeft.y);
	window.draw(r1);

	auto r2 = sf::CircleShape(2);
	r2.setFillColor(sf::Color::Red);
	r2.setPosition(c.bottomLeft.x, c.bottomLeft.y);
	window.draw(r2);

	auto r3 = sf::CircleShape(2);
	r3.setFillColor(sf::Color::Green);
	r3.setPosition(c.bottomRight.x, c.bottomRight.y);
	window.draw(r3);

	auto r4 = sf::CircleShape(2);
	r4.setFillColor(sf::Color::Yellow);
	r4.setPosition(c.topRight.x, c.topRight.y);
	window.draw(r4);
}

unsigned int generateLevel(int l) {
	levelInfo info = generateLevelInfo(l);
	SpaceshipFactory::setDifficulty(info.aggressiveness, info.sluggishness);
	for (int i = 0; i <info.patrols; i++) { // generate enemies
		EnemySpaceship* e = SpaceshipFactory::generateRandomEnemy(0);//instantiate enemies
		ScreenThing::spaceships->push_back(e);
		ScreenThing::enemySpaceships->push_back(e);
	}
	for (int i = 0; i < info.fighters; i++) { // generate enemies
		EnemySpaceship* e = SpaceshipFactory::generateRandomEnemy(1);//instantiate enemies
		ScreenThing::spaceships->push_back(e);
		ScreenThing::enemySpaceships->push_back(e);
	}
	for (int i = 0; i < info.cruisers; i++) { // generate enemies
		EnemySpaceship* e = SpaceshipFactory::generateRandomEnemy(2);//instantiate enemies
		ScreenThing::spaceships->push_back(e);
		ScreenThing::enemySpaceships->push_back(e);
	}
	for (int i = 0; i < info.carriers; i++) { // generate enemies
		EnemySpaceship* e = SpaceshipFactory::generateRandomEnemy(3);//instantiate enemies
		ScreenThing::spaceships->push_back(e);
		ScreenThing::enemySpaceships->push_back(e);
	}
	for (int i = 0; i < info.blackHoles; i++) { // generate blackholes
		ScreenThing::blackHole = new BlackHole(size * (1.f / 2.f));
	}
	return ++l;
}

void getUserInput(Spaceship* player) {
	//check keypresses
	point moveVector = { 0,0 };
	float rotation = 0;
	bool move = false;
	bool rotate = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		moveVector.x = -1;
		//player->move(moveVector, windowBox, spaceships);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		moveVector.x = 1;
		//player->move(moveVector, windowBox, spaceships);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		moveVector.y = -1;
		//player->move(moveVector, windowBox, spaceships);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		moveVector.y = 1;
		//player->move(moveVector, windowBox, spaceships);
	}
	player->move(moveVector);
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
			player->fireBullet();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket)) {
		if (player->torpedoReady()) {
			player->fireTorpedo();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon)) {
		if (player->mineReady()) {
			player->layMine();
		}
	}
}

void checkPause(sf::Event event) {
	unsigned long long int count = 0;
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code==sf::Keyboard::Space) {
			paused = !paused;
			//std::cout << "Pause toggled" << std::endl;
			if (paused) {
				window.draw(pauseText);
				window.draw(controlsText);
			}
		}
	}
	while (paused) {
		
		sf::Event newEvent;
		while (window.pollEvent(newEvent)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (count == 0) {
				window.display();
			}
			checkPause(newEvent);
		}
		count++;
	}
}

unsigned int getHighScore() {
	const static int maxSize = 5000;
	char buf[maxSize] = {""};
	char* token;
	char* nextToken;
	unsigned int max = 0;
	std::ifstream log(logFile);
	std::vector<unsigned int> scores;
	if (log) {//if file exists
		//std::cout << "file exists" << std::endl;
		log.read(buf, maxSize);
		//std::cout << "buf: " << buf << std::endl;
		token = strtok_s(buf, "\n",&nextToken);
		//std::cout << "init token: " << token << std::endl;
		scores.push_back(atoi(token));
		while (token != NULL) {
			//std::cout << "token!=NULL" << std::endl;
			token = strtok_s(NULL, "\n",&nextToken);
			if (token != NULL) {
				scores.push_back(atoi(token));
			}
			//std::cout << "token: " << token << " int token: " << int(token) << std::endl;
		}
		for (auto s : scores) {
			//std::cout << "score: " << s << std::endl;
			if (s > max) {
				max = s;
			}
		}
		return max;
	}
	else {
		return 0;
	}
}

void logScore() {
	std::ofstream log;
	std::string scoreString = std::to_string(prevScore);
	log.open(logFile, std::ofstream::out | std::ofstream::app);
	log << scoreString << "\n";
	log.close();
}

void intro(sf::Font f, sf::Text controlsText) {
	std::vector<std::string> introLines = { "YOU ARE HUMANITY'S LAST HOPE.",
											"Scientists have determined that a black hole will appear in the area.",
											"Nearby populations are fleeing the inevitable catastrophe while an enemy armada pursues them.",
											"Your mission is to defeat the enemy armada", 
											"and slow the black hole's growth for as long as possible",
											"to provide the refugees enough time to escape.",
											"Good luck."};
	//std::string controlsTextLines = "CONTROLS:\nMOVEMENT: WASD\nROTATE LEFT: Q\nROTATE RIGHT: E\nFIRE CANNON: [\nFIRE TORPEDO: ]\nDROP MINE: ;\nPAUSE: [SPACE]";
	int timeCount = 5000;
	sf::Text introText = sf::Text();
	//sf::Text controlsText = sf::Text(controlsTextLines,f);
	sf::Text enter = sf::Text("Press [Enter] to continue", f);
	introText.setFont(f);
	
	
	
	enter.setOrigin(enter.getLocalBounds().width / 2, enter.getLocalBounds().height / 2);
	enter.setPosition(window.getSize().x/2, window.getSize().y * 5.f / 8.f);
	for (auto s : introLines) {
		introText.setString(s);
		introText.setOrigin(introText.getLocalBounds().width / 2, introText.getLocalBounds().height / 2);
		introText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
		
		window.draw(introText);
		window.draw(enter);
		window.draw(controlsText);
		window.display();
		bool done = false;
		while (!done) {
			sf::Event event;
			done = false;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				else if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Enter) {
						done = true;
						break;
					}

				}
			}
			
		}
		window.clear();
	}
}

template <typename T>

void resetVector(std::vector<T> *vect){
	for (auto it = vect->begin(); it != vect->end();) {
		delete (*it);
		it++;
	}
	vect->clear();
}

void resetAll() {
	window.clear();
	delete ScreenThing::blackHole;
	ScreenThing::blackHole = nullptr;
	resetVector(&spaceships);
	resetVector(&projectiles);
	resetVector(&explosions);
	resetVector(&powerUps);
	enemySpaceships.clear();
	masterClock.restart();

}

int main()
{
	if(!count){ srand(time(0)); }
	
	bool logged = false;
	bool levelUpActive = false;
	int initPlayerLevel = 0;
	int playerLevel = initPlayerLevel;
	int level = 0;
	int enemiesSpawnPeriod = 1250;
	int enemiesSpawnNumber = 3;
	int powerUpsInit = 4;
	int powerUpSpawnNumber = 2;
	int powerUpPeriod = 1500;
	int warningFlashPeriod = 50;
	bool warningState = false;

	std::string controlsTextLines = "CONTROLS:\nMOVEMENT: WASD\nROTATE LEFT: Q\nROTATE RIGHT: E\nFIRE CANNON: [\nFIRE TORPEDO: ]\nDROP MINE: ;\nPAUSE: [SPACE]";
	
	controlsText = sf::Text(controlsTextLines, font);
	

	highScore = getHighScore();
	font.loadFromFile("pixel_font.ttf");

	healthReadout.setFont(font);
	healthReadout.setPosition(0, 0);
	healthReadout.setCharacterSize(30);
	healthReadout.setFillColor(sf::Color::Red);
	healthReadout.setStyle(sf::Text::Bold);
	
	score.setFont(font);
	score.setPosition(250, 0);
	score.setCharacterSize(30);
	score.setFillColor(sf::Color::Red);
	score.setStyle(sf::Text::Bold);

	highScoreText.setFont(font);
	highScoreText.setPosition(450, 0);
	highScoreText.setCharacterSize(30);
	highScoreText.setFillColor(sf::Color::Red);
	highScoreText.setStyle(sf::Text::Bold);

	playerStatus.setFont(font);
	playerStatus.setPosition(window.getSize().x-340, window.getSize().y -140);
	
	playerStatus.setCharacterSize(30);
	playerStatus.setFillColor(sf::Color::Red);
	playerStatus.setStyle(sf::Text::Bold);

	levelDisplay.setFont(font);
	levelDisplay.setPosition(750, 0);
	levelDisplay.setCharacterSize(30);
	levelDisplay.setFillColor(sf::Color::Red);
	levelDisplay.setStyle(sf::Text::Bold);
	
	warningDisplay.setFont(font);
	warningDisplay.setPosition(900, 0);
	warningDisplay.setCharacterSize(30);
	warningDisplay.setFillColor(sf::Color::Red);
	warningDisplay.setStyle(sf::Text::Bold);

	pauseText.setString("Paused");
	pauseText.setFont(font);
	pauseText.setCharacterSize(60);
	pauseText.setFillColor(sf::Color::Red);
	pauseText.setStyle(sf::Text::Bold);
	pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
	pauseText.setPosition(size.x / 2, size.y / 2);

	controlsText.setPosition(10, window.getSize().y * 5.f / 8.f);
	

	endText.setFont(font);
	endText.setCharacterSize(60);
	endText.setFillColor(sf::Color::Red);
	endText.setStyle(sf::Text::Bold);
	
	intro(font, controlsText);
	controlsText.setFillColor(sf::Color::Red);
	FrameClock::Init(frame);
	FrameClock powerUpTimer;
	FrameClock enemyTimer;
	FrameClock warningFlasher;

	background.loadFromFile("background1.png");
	backgroundTexture.loadFromImage(background);
	bSprite.setTexture(backgroundTexture);
	point scale = { size.x / backgroundTexture.getSize().x,size.y / backgroundTexture.getSize().y };
	bSprite.setScale(scale.x, scale.y);
	
	ScreenThing::InitGame("shield.png", windowBox, size,&spaceships,&enemySpaceships,&projectiles, &explosions, &powerUps);
	
	PatrolShip::Init("patrolship.png");
	Fighter::Init("fighter.png");
	Cruiser::Init("cruiser.png");

	Carrier::Init("carrier.png");
	Mini::Init("mini.png");
	
	Bullet::Init("bullet.png");
	Torpedo::Init("torpedo.png");
	Mine::Init("mine.png");
	GuidedMissile::Init("guidedmissile.png");

	Explosion::Init("explosion.png");
	TextExplosion::Init("pixel_font.ttf", sf::Color::Green);
	SpawnExplosion::Init("spawn.png");
	PowerUp::Init("powerup.png");
	Upgrade::Init("levelup.png");
	
	BlackHole::Init("blackhole.png");
	PowerUpFactory::Init(ScreenThing::powerUps, size);
	SpaceshipFactory::Init(1, 0,size, ScreenThing::spaceships);
	spaceships.push_back(SpaceshipFactory::generatePlayer(0));//instantiate player
	
	Spaceship* player = ScreenThing::spaceships->at(0);
	EnemySpaceship::Init(player); // initialize enemySpaceships with player as the target
	point randpos;
	player->takeDamage(-130);
	level = generateLevel(level);
	
	for (float i = 0; i < powerUpsInit; i++) { // generate powerups
		ScreenThing::powerUps->push_back(PowerUpFactory::generateRandom());
	}
	
	powerUpTimer.restart();
	while (window.isOpen())
	{
		
		masterClock.restart();
		FrameClock::updateTime(frame);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (player->isActive()) {
				checkPause(event);
			}
		}
		
		
		//maybe generate powerups
	
		if (powerUpTimer.getTime() > powerUpPeriod && player->isActive()) {
			for (int i = 0; i < powerUpSpawnNumber; i++) { // generate powerups
				ScreenThing::powerUps->push_back(PowerUpFactory::generateRandom());
			}
			//ScreenThing::powerUps->push_back(PowerUpFactory::generateSpecificTypeRandom(4));//guaranteed to generate health powerup
			powerUpTimer.restart();
		}
		if ((enemyTimer.getTime() > enemiesSpawnPeriod && ScreenThing::enemySpaceships->size() == 0) && player->isActive()) {
			level = generateLevel(level);
			enemyTimer.restart();
		}
		if (player->isActive()) {
			getUserInput(player); //manage user input
			if (playerLevel < player->getLevel()) {
				levelUpActive = false;
			}
			playerLevel = player->getLevel();
		}
		if (player->isActive() && player->getScore() >= thresholds[player->getLevel()] && !levelUpActive && player->getLevel() < MAX_LEVEL) {
			ScreenThing::powerUps->push_back(PowerUpFactory::generateSpecificTypeRandom(-1)); //generate levelup powerup
			levelUpActive = true;
		}
		if (!player->isActive()&& !logged) {
			logScore();
			logged = true;
			highScore = getHighScore();
		}
		
		//make sure the frame doesn't draw too fast
		//std::cout << "Player level: " << player->getLevel() << std::endl;
		while (masterClock.getElapsedTime().asMicroseconds() < framePeriodMicroSeconds) {}
		window.clear();
		window.draw(bSprite);//draw the background
		//manage all the spaceships and projectiles
		static int code;
		int loc = 0;
		
		if (ScreenThing::blackHole != nullptr) {
			ScreenThing::blackHole->eatSpaceships();
			ScreenThing::blackHole->eatProjectiles();
			ScreenThing::blackHole->eatPowerUps();
			window.draw(ScreenThing::blackHole->getSprite());
		}

		for (auto it = ScreenThing::enemySpaceships->begin(); it != ScreenThing::enemySpaceships->end();) { //iterate through enemy spaceships and remove the dead ones (doesn't delete; next block does that) and AI the alive ones
			if ((*it) && ScreenThing::spaceships->size() > 1) {
				if ((*it)->isActive()) {
					int vSize = ScreenThing::enemySpaceships->size();
					
					(*it)->specialMove();
					if (ScreenThing::enemySpaceships->size() != vSize) { // if carrier spawns new enemies and resizes vector
						//std::cout << "resized" << std::endl;
						it = ScreenThing::enemySpaceships->begin();
						it += loc;
					}
					++it;
				}
				else {
					//std::cout << "erasing enemy spaceship at " << *it << std::endl;
					it = ScreenThing::enemySpaceships->erase(it);
				}
			}
			loc++;
		}
		//std::cout << "spaceships: " << ScreenThing::spaceships->size() << std::endl;
		for (auto it = ScreenThing::spaceships->begin(); it != ScreenThing::spaceships->end();) { //iterate through all spaceships and draw the alive ones and explode the dead ones
			if (*it) {
				if ((*it)->isActive()) {
					window.draw((*it)->getSprite());
					if ((*it)->shieldActive()) {
						window.draw((*it)->getShield());
					}
					//showBounds(*it);
					++it;
				}
				else if(*it != player) {
					(*it)->explode();
					
					//std::cout << "erasing spaceship at " << *it << std::endl;
					delete (*it);
					(*it) = nullptr;
					//std::cout << "removing it from spaceships" << std::endl;
					it = ScreenThing::spaceships->erase(it); //resize vector and adjust iterator
					
				}
				else {
					break;
				}
			}
		}
		
		//std::cout << "projectiles:" << ScreenThing::projectiles->size() << std::endl;
		//std::cout << "m projectiles:" << projectiles.size() << std::endl;
		for (auto it = ScreenThing::projectiles->begin(); it != ScreenThing::projectiles->end();) {//iterate through projectiles and draw the active ones and explode/disappear the dead/OOB ones
			if (!(*it)->move()) {
				if ((*it)->getCounter() > 1) {
					window.draw((*it)->getSprite());
					//showBounds(*it);
				}
				++it;
			}
			else {
				if ((*it)->getCollision()) {
					(*it)->explode();
				}
				delete* it;
				(*it) = nullptr;
				//std::cout << "erasing projectile at " << *it << std::endl;
				it = ScreenThing::projectiles->erase(it);
			}
		}
			
		for (auto it = ScreenThing::explosions->begin(); it != ScreenThing::explosions->end();) { //draw/disappear all explosions
			if ((*it)->isActive()) {
				//std::cout << "drawing explosion" << std::endl;
				window.draw((*it)->getSprite());
				++it;
			}
			else {
				delete* it;
				(*it) = nullptr;
				//std::cout << "erasing explosion at " << it[0] << std::endl;
				it = ScreenThing::explosions->erase(it);
			}
		}
		
		for (auto it = ScreenThing::powerUps->begin(); it != ScreenThing::powerUps->end();) { //iterate through all powerups, draw the alive ones and explode the dead ones
			if ((*it)->isActive()) {
				window.draw((*it)->getSprite());
				//showBounds(*it);
				++it;
			}
			else if (player->isActive()){
				if (!(*it)->eaten) {
					(*it)->explode();
				}
				if ((*it)->isUpgrade()) {
					player = ScreenThing::spaceships->at(0); //need to fix the pointer to player so that the user inputs still map to the correct spaceship
				}
				delete* it;
				//std::cout << "erasing powerup at " << *it << std::endl;
				it = ScreenThing::powerUps->erase(it);
			}
			else {
				if (!(*it)->eaten) {
					(*it)->explode();
				}
				delete* it;
				//std::cout << "erasing powerup at " << *it << std::endl;
				it = ScreenThing::powerUps->erase(it);
			}
		}
		//Spaceship* player = spaceships[0];
		//EnemySpaceship::Init(player); // initialize enemySpaceships with player as the target
		if (player->isActive()) {
			healthReadout.setString("Player health: " + std::to_string(player->getHealth()));
			score.setString("Score: " + std::to_string(player->getScore()));
			prevScore = player->getScore();
			std::string statusString = "MINES: " + std::to_string(player->getMineCount()) +
				"\nGUIDED MISSILES: " + std::to_string(player->getGuidedMissileCount());
			if (player->getChasedByCount()) {
				statusString.append("\nLOCKED ENEMY MISSILES: " + std::to_string(player->getChasedByCount()));
			}
			playerStatus.setString(statusString);

		}
		else {
			healthReadout.setString("Player health: 0");
			score.setString("Score: " + std::to_string(prevScore));
		}
		if (prevScore > highScore) {
			highScore = prevScore;
		}
		highScoreText.setString("High Score: " + std::to_string(highScore));
		levelDisplay.setString("Level: " + std::to_string(level));
		if (level == BLACKHOLE_LEVEL) {
			if (warningFlasher.getTime() > warningFlashPeriod && !warningState) {
				warningDisplay.setString("BLACK HOLE IN CENTER IMMINENT");
				warningFlasher.restart();
				warningState = true;
			}
			else if(warningFlasher.getTime() > warningFlashPeriod &&warningState) {
				warningDisplay.setString("");
				warningFlasher.restart();
				warningState = false;
			}
			
		}
		else if (ScreenThing::blackHole == nullptr) {
			warningDisplay.setString("");
		}
		else {
			warningDisplay.setString("BLACK HOLE MASS: " + std::to_string(ScreenThing::blackHole->getMass()));
		}
		if (!player->isActive()) {
			if (prevScore == highScore) {
				endText.setString("New High Score: " + std::to_string(highScore) + "! Press Esc to exit.\n        or [Enter] to play again.");
			}
			else {

				endText.setString("Game Over. Press [Esc] to exit\n   or [Enter] to play again.");
			}
			endText.setOrigin(endText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
			endText.setPosition(size.x / 2, size.y / 2);
			window.draw(endText);

		}
		window.draw(highScoreText);
		window.draw(healthReadout);
		window.draw(levelDisplay);
		window.draw(score);
		window.draw(warningDisplay);
		window.draw(playerStatus);
		if (!player->isActive()&& ScreenThing::explosions->size()==0) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				else if (event.type == sf::Event::KeyPressed) {
					if(event.key.code == sf::Keyboard::Escape){ 
						window.close(); 
					}
					if (event.key.code == sf::Keyboard::Enter) {
						resetAll();
						main();
						count++;
					}
					
				}
			}
		}
		
		window.display(); //show the window
		frame++; //increment the frame counter
	}
	//std::cout << "player is dead." << std::endl;
	return 0;
}