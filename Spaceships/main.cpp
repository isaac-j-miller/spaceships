#include <SFML/Graphics.hpp>
#include "Bullet.h"//includes projectile
#include "Torpedo.h"
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
#include <iostream>
#include <fstream>
#include <chrono>
const int thresholds[] = { 4000, 10000, 20000 };
const int MAX_LEVEL = 2;
const int SPACESHIP_TYPES = MAX_LEVEL + 1;
const char* logFile = "scores.txt";
unsigned long long int frame = 0;
unsigned int highScore;
//const float framerate = 512; //fps
//float framePeriodMicroSeconds = (1 / framerate) * 10E6;
unsigned int prevScore = 0;
bool paused = false;
const point size = { 1280, 960 };
const box windowBox = { { 0,0 } ,{0,size.y},{size.x,size.y},{size.x,0} };
sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Spaceships");
sf::Font font;
sf::Text healthReadout;
sf::Text score;
sf::Text highScoreText;
sf::Text pauseText;
sf::Text endText;
sf::Text levelDisplay;
sf::Image background;
sf::Texture backgroundTexture;
sf::Sprite bSprite;
sf::Clock masterClock;
std::string shieldImageFileName;
std::vector<Spaceship*> spaceships;
std::vector<EnemySpaceship*> enemySpaceships;
std::vector<Projectile*> projectiles;
std::vector<Explosion*> explosions;
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
	SpaceshipFactory::setDifficulty(info.difficulty);
	for (int i = 0; i <info.patrols; i++) { // generate enemies
		EnemySpaceship* e = SpaceshipFactory::generateRandomEnemy(0);//instantiate enemies
		spaceships.push_back(e);
		enemySpaceships.push_back(e);
	}
	for (int i = 0; i < info.fighters; i++) { // generate enemies
		EnemySpaceship* e = SpaceshipFactory::generateRandomEnemy(1);//instantiate enemies
		spaceships.push_back(e);
		enemySpaceships.push_back(e);
	}
	for (int i = 0; i < info.cruisers; i++) { // generate enemies
		EnemySpaceship* e = SpaceshipFactory::generateRandomEnemy(2);//instantiate enemies
		spaceships.push_back(e);
		enemySpaceships.push_back(e);
	}
	for (int i = 0; i < info.carriers; i++) { // generate enemies
		EnemySpaceship* e = SpaceshipFactory::generateRandomEnemy(3);//instantiate enemies
		spaceships.push_back(e);
		enemySpaceships.push_back(e);
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
}

void checkPause(sf::Event event) {
	unsigned long long int count = 0;
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code==sf::Keyboard::Space) {
			paused = !paused;
			//std::cout << "Pause toggled" << std::endl;
		}
	}
	while (paused) {
		window.draw(pauseText);
		
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

int main()
{
	unsigned int seed = (std::chrono::system_clock::now().time_since_epoch().count() - 15672112962443214) / 10E6;
	std::cout << "seed:" << seed<< std::endl;
	srand(seed);
	for (int i = 0; i < 20; i++) {
		std::cout << rand() << std::endl;
	}
	bool logged = false;
	bool levelUpActive = false;
	int initPlayerLevel = 2;
	int playerLevel = initPlayerLevel;
	int level = 5;
	//int numEnemies = 6;
	int enemiesSpawnPeriod = getPeriod(1500);
	int enemiesSpawnNumber = 2;
	//int difficulty = 2;
	//int difficultyIncrement = 1;
	int powerUpsInit = 4;
	int powerUpSpawnNumber = 1;
	int powerUpPeriod = getPeriod(1250);
	//unsigned int levelUpScore = 10000;
	highScore = getHighScore();
	
	//std::cout << "Frame Period is " << framePeriodMicroSeconds << " us" << std::endl;
	//std::cout << "spaceship size: " << sizeof(Spaceship) << " enemyspaceship size: " << sizeof(EnemySpaceship) << " patrolship size: " << sizeof(PatrolShip) << "enemypatrolship size: " << sizeof(EnemyPatrolShip) << std::endl;
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

	levelDisplay.setFont(font);
	levelDisplay.setPosition(750, 0);
	levelDisplay.setCharacterSize(30);
	levelDisplay.setFillColor(sf::Color::Red);
	levelDisplay.setStyle(sf::Text::Bold);

	pauseText.setString("Paused");
	pauseText.setFont(font);
	pauseText.setCharacterSize(60);
	pauseText.setFillColor(sf::Color::Red);
	pauseText.setStyle(sf::Text::Bold);
	pauseText.setOrigin(pauseText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
	pauseText.setPosition(size.x / 2, size.y / 2);

	endText.setFont(font);
	endText.setCharacterSize(60);
	endText.setFillColor(sf::Color::Red);
	endText.setStyle(sf::Text::Bold);
	

	FrameClock::Init(frame);
	FrameClock powerUpTimer;
	FrameClock enemyTimer;

	background.loadFromFile("background1.png");
	backgroundTexture.loadFromImage(background);
	bSprite.setTexture(backgroundTexture);
	point scale = { size.x / backgroundTexture.getSize().x,size.y / backgroundTexture.getSize().y };
	bSprite.setScale(scale.x, scale.y);

	//Spaceship::Init("shield.png", &spaceships, &enemySpaceships, &projectiles, &explosions, windowBox); 
	
	ScreenThing::InitGame("shield.png", windowBox, size,&spaceships,&enemySpaceships,&projectiles, &explosions, &powerUps);

	PatrolShip::Init("patrolship.png");
	Fighter::Init("fighter.png");
	Cruiser::Init("cruiser.png");

	Carrier::Init("carrier.png");
	Mini::Init("mini.png");
	
	Bullet::Init("bullet.png");
	Torpedo::Init("torpedo.png");
	Explosion::Init("explosion.png");
	PowerUp::Init("powerup.png");
	Upgrade::Init("levelup.png");
	TextExplosion::Init("pixel_font.ttf", sf::Color::Green);
	
	PowerUpFactory::Init(&powerUps, size);
	SpaceshipFactory::Init(1, size, &spaceships);
	spaceships.push_back(SpaceshipFactory::generatePlayer(initPlayerLevel));//instantiate player
	
	static Spaceship* player = spaceships.at(0);
	//Projectile::Init(&explosions, &spaceships, size);
	EnemySpaceship::Init(player); // initialize enemySpaceships with player as the target
	point randpos;
	//bool c = false; //used to make sure RNG doesn't place 2 spaceships in the same spot
	
	level = generateLevel(level);
	
	for (float i = 0; i < powerUpsInit; i++) { // generate powerups
		powerUps.push_back(PowerUpFactory::generateRandom());
	}
	
	powerUpTimer.restart();
	while (window.isOpen())
	{
		//if (!player ->isActive()) {
		//	break;
		//}
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
				powerUps.push_back(PowerUpFactory::generateRandom());
			}
			powerUps.push_back(PowerUpFactory::generateSpecificTypeRandom(4));//guaranteed to generate health powerup
			powerUpTimer.restart();
		}
		if ((enemyTimer.getTime() > enemiesSpawnPeriod && enemySpaceships.size() == 0) && player->isActive()) {
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
			powerUps.push_back(PowerUpFactory::generateSpecificTypeRandom(-1)); //generate levelup powerup
			levelUpActive = true;
		}
		if (!player->isActive()&& !logged) {
			logScore();
			logged = true;
			highScore = getHighScore();
		}
		
		//make sure the frame doesn't draw too fast
		//std::cout << "Player level: " << player->getLevel() << std::endl;
		
		window.clear();
		window.draw(bSprite);//draw the background
		//manage all the spaceships and projectiles
		static int code;
		int loc = 0;
		for (auto it = enemySpaceships.begin(); it != enemySpaceships.end();) { //iterate through enemy spaceships and remove the dead ones (doesn't delete; next block does that) and AI the alive ones
			
			if ((*it) && spaceships.size() > 1) {
				if ((*it)->isActive()) {
					int vSize = enemySpaceships.size();
					
					(*it)->specialMove();
					if (enemySpaceships.size() != vSize) { // if carrier spawns new enemies and resizes vector
						//std::cout << "resized" << std::endl;
						it = enemySpaceships.begin();
						it += loc;
					}
					++it;
				}
				else {
					//std::cout << "erasing enemy spaceship at " << *it << std::endl;
					it = enemySpaceships.erase(it);
				}
			}
			loc++;
		}

		for (auto it = spaceships.begin(); it != spaceships.end();) { //iterate through all spaceships and draw the alive ones and explode the dead ones
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
					delete *(it);
					(*it) = nullptr;
					//std::cout << "removing it from spaceships" << std::endl;
					it = spaceships.erase(it); //resize vector and adjust iterator
					
				}
				else {
					break;
				}
			}
		}


		for (auto it = projectiles.begin(); it != projectiles.end();) {//iterate through projectiles and draw the active ones and explode/disappear the dead/OOB ones
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
				delete *it;
				//std::cout << "erasing projectile at " << *it << std::endl;
				it = projectiles.erase(it);
			}
		}
		for (auto it = explosions.begin(); it != explosions.end();) { //draw/disappear all explosions
			if ((*it)->isActive()) {
				//std::cout << "drawing explosion" << std::endl;
				window.draw((*it)->getSprite());
				++it;
			}
			else {
				delete *it;
				//std::cout << "erasing explosion at " << it[0] << std::endl;
				it = explosions.erase(it);
			}
		}
		
		for (auto it = powerUps.begin(); it != powerUps.end();) { //iterate through all powerups, draw the alive ones and explode the dead ones
			if ((*it)->isActive()) {
				window.draw((*it)->getSprite());
				//showBounds(*it);
				++it;
			}
			else if (player->isActive()){
				(*it)->explode();
				if ((*it)->isUpgrade()) {
					player = spaceships.at(0); //need to fix the pointer to player so that the user inputs still map to the correct spaceship
				}
				delete *it;
				//std::cout << "erasing powerup at " << *it << std::endl;
				it = powerUps.erase(it);
			}
		}
		//Spaceship* player = spaceships[0];
		//EnemySpaceship::Init(player); // initialize enemySpaceships with player as the target
		if (player->isActive()) {
			healthReadout.setString("Player health: " + std::to_string(player->getHealth()));
			score.setString("Score: " + std::to_string(player->getScore()));
			prevScore = player->getScore();

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
		if (!player->isActive()) {
			if (prevScore == highScore) {
				endText.setString("New High Score: " + std::to_string(highScore) + "! Press Esc to exit.");
			}
			else {

				endText.setString("Game Over. Press Esc to exit.");
			}
			endText.setOrigin(endText.getLocalBounds().width / 2, pauseText.getLocalBounds().height / 2);
			endText.setPosition(size.x / 2, size.y / 2);
			window.draw(endText);

		}
		window.draw(highScoreText);
		window.draw(healthReadout);
		window.draw(levelDisplay);
		window.draw(score);
		if (!player->isActive()&& explosions.size()==0) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				else if (event.type == sf::Event::KeyPressed) {
					if(event.key.code == sf::Keyboard::Escape){ 
						window.close(); 
					}
					
				}
			}
		}
		while (masterClock.getElapsedTime().asMicroseconds() < FRAMEPERIOD) {}
		std::cout << masterClock.getElapsedTime().asMicroseconds() << std::endl;
		window.display(); //show the window
		frame++; //increment the frame counter
	}
	//std::cout << "player is dead." << std::endl;
	return 0;
}