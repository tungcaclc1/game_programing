#include"Animation.h"
#include<SFML/Audio.hpp>
#include<SFML/System.hpp>
#include<iostream>
#include<vector>
#include<stdlib.h>    
#include<time.h>  
#include<cstdlib>
#include<sstream>;
#include<string>;


using namespace std;
using namespace sf;


int main()
{
	srand(time(NULL));

	//Set a window
	RenderWindow window(VideoMode(800, 600), "Game");
	window.setFramerateLimit(60);


	//Create player
	RectangleShape player(Vector2f(80.0f, 50.0f));
	RectangleShape mask(Vector2f(10.f, 10.f));

	player.setPosition(400.0f, 300.0f);
	player.setOrigin(40.0f, 25.0f);
	mask.setOrigin(5.f, 5.f);

	Texture playerTexture,tMask;
	playerTexture.loadFromFile("player.png");
	playerTexture.setSmooth(false);
	player.setTexture(&playerTexture);
	
	tMask.loadFromFile("collisionMask.png");
	mask.setTexture(&tMask);

	bool isDrawPlayer = true;
	int playerSpawn = 0;
	int collidedX, collidedY;

	Animation animation(&playerTexture, Vector2u(4, 1), 0.1f);


	//Clock for player animation
	float deltaTime = 0.0f;
	Clock clock;
	
	
	//BOss
	Texture tBoss;
	tBoss.loadFromFile("boss.png");
	RectangleShape boss(Vector2f(160.f, 300.f));
	boss.setOrigin(80.f, 150.f);
	boss.setTexture(&tBoss);
	boss.setPosition(950, 300);
	boss.setRotation(-90);

	Texture tRocket;
	tRocket.loadFromFile("rocket.png");
	RectangleShape rocket(Vector2f(150.f, 100.f));
	rocket.setOrigin(25.f, 50.f);
	rocket.setTexture(&tRocket);
	rocket.setPosition(490, 300);
	rocket.setScale(0.3f, 0.3f);

	vector<RectangleShape> rockets;
	
	int bossShootTime=0;
	int bossMoveTime = 0;
	int direction = 1;
	int health = 5;

	Texture tMaskBoss;
	tMaskBoss.loadFromFile("collisionMask.png");
	RectangleShape maskBoss(Vector2f(20.f, 20.f));
	maskBoss.setTexture(&tMaskBoss);
	maskBoss.setOrigin(10.f, 10.f);
	
	
	//Create exploion          
	RectangleShape explosion(Vector2f(50.f, 50.f));
	vector<RectangleShape> explosions;
	explosions.push_back(explosion);


	Texture explosionTexture;
	explosionTexture.loadFromFile("explosion.png");
	explosion.setTexture(&explosionTexture);
	explosions[0].setTexture(&explosionTexture);

	Animation exploAni(&explosionTexture, Vector2u(5, 1), 0.1f);
	vector<Animation> exploAniArr;
	exploAniArr.push_back(exploAni);

	int countFrame = 0;


	//Create a array of animation for enemy
	RectangleShape enemy(Vector2f(60.0f, 50.0f));
	vector<RectangleShape> enemies;
	enemies.push_back(enemy);

	Texture enemyTexture;
	enemyTexture.loadFromFile("enemy_flying.png");

	Animation enemy1Ani(&enemyTexture, Vector2u(9, 1), 0.1f);
	vector<Animation> enemy1AniArr;

	enemy1AniArr.push_back(enemy1Ani);
	enemies[0].setTexture(&enemyTexture);
	int spawnTime = 0;
	int maxSpawnTime = 200;

	//Rock enemy
	RectangleShape rock(Vector2f(30.f, 30.f));
	vector<RectangleShape> rocks;
	rocks.push_back(rock);

	Texture rockTexture;
	rockTexture.loadFromFile("rock_small.png");
	rockTexture.setSmooth(true);

	Animation rockAni(&rockTexture, Vector2u(16, 1), 0.05f);
	vector<Animation> rockAniArr;

	rockAniArr.push_back(rockAni);
	rocks[0].setTexture(&rockTexture);
	
	int spawnRock = 0;
	int spawnMaxRock = 100;

	//Bullet
	Texture tBullet;
	tBullet.loadFromFile("fire_red.png");
	tBullet.setSmooth(true);
	Sprite sBullet(tBullet, IntRect(0, 0, 35, 50));
	sBullet.setScale(0.5f, 0.5f);
	sBullet.setRotation(90);
	sBullet.setOrigin(17, 25);
	vector<Sprite> bullets;
	bullets.push_back(Sprite(sBullet));
	int timeToShoot = 0;


	//Set shoot sound
	SoundBuffer shootSound;
	shootSound.loadFromFile("shootSoundgg.wav");
	Sound shoot;
	shoot.setBuffer(shootSound);

	
	//Set enemy die sound
	SoundBuffer dieSound;
	dieSound.loadFromFile("diesoundgg.wav");
	Sound die;
	die.setBuffer(dieSound);


	//Set gameover sound
	SoundBuffer overSound;
	overSound.loadFromFile("gameover.wav");
	Sound gameoverSound;
	gameoverSound.setBuffer(overSound);
	bool isPlay = true;
	bool isDrawLastEx = true;
	bool isMain = false;

	//load music theme song
	Music music,intro;
	music.openFromFile("themeSong.ogg");
	music.setVolume(50);
	music.play();
	music.setLoop(true);

	//Set a bacground
	Texture bg;
	bg.loadFromFile("bg2.jpg");
	Sprite background(bg);
	background.setScale(0.5f, 0.5f);

	Texture bgOver;
	bgOver.loadFromFile("bg2blur.jpg");
	Sprite backOver(bgOver);
	backOver.setScale(0.5f, 0.5f);
	int countBgOver = 0;

	//Load font
	Font font;
	font.loadFromFile("font.ttf");
	Text point;
	point.setFont(font);
	int score = 0;
	point.setString("Score: 0");
	point.setCharacterSize(15);

	Text life;
	life.setFont(font);
	int lifepoint = 3;
	life.setString("Life: 3");
	life.setCharacterSize(15);
	life.setPosition(0.f, 20.f);

	Text over;
	over.setFont(font);
	over.setString("Game over");
	over.setCharacterSize(60);
	over.setPosition(250.f, 250.f);

	Text esc;
	esc.setFont(font);
	esc.setString("Press ESC quit the game");
	esc.setCharacterSize(20);
	esc.setPosition(280.f, 320.f);

	//Main loop
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		

		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape) {
					window.close();
					break;
				}
				if (event.key.code == Keyboard::LControl) {
					isMain = true;
					break;
				}
			}
		}
		
		//Control the player
		if (isMain == true) {
			int speed = 0;
			Vector2f position = player.getPosition();

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				if (position.x == 40)
					speed = 0;
				else speed = 2;
				player.move(-speed, 0.0f);
				position.x++;
			}


			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				if (position.x == 760)
					speed = 0;
				else speed = 2;
				player.move(speed, 0.0f);
				position.x--;
			}


			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (position.y == 26)
					speed = 0;
				else speed = 2;
				player.move(0.0f, -speed);
				position.y++;
			}


			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				if (position.y == 574)
					speed = 0;
				else speed = 2;
				player.move(0.0f, speed);
				position.y--;
			}

			//collision mask for player
			mask.setPosition(player.getPosition().x, player.getPosition().y);
			
			//Shoot
			if (timeToShoot < 20) timeToShoot++;
			if (Keyboard::isKeyPressed(Keyboard::Space) && timeToShoot >= 20)
			{
				timeToShoot = 0;
				sBullet.setPosition(position.x + 40, position.y + 2);
				bullets.push_back(Sprite(sBullet));
				shoot.play();
			}
			for (int i = 0; i < bullets.size(); i++)
			{
				bullets[i].move(5.0f, 0);
				if ((bullets[i].getPosition().y <= 0) || (bullets[i].getPosition().x >= 800))
				{
					bullets.erase(bullets.begin() + i);
				}
			}


			//Make animated player
			animation.Update(0, deltaTime, true);
			player.setTextureRect(animation.uvRect);


			//Make animated explosion
			exploAni.Update(0, deltaTime, true);
			explosion.setTextureRect(exploAni.uvRect);

			//Make animated enemies
			if (spawnTime < maxSpawnTime)
				spawnTime++;

			if (spawnTime >= maxSpawnTime)
			{
				enemy.setPosition(800.0f, 10 + rand() % 501 + 50);
				enemy.setTexture(&enemyTexture);
				enemy.setOrigin(30.f, 25.f);
				enemies.push_back(RectangleShape(enemy));
				enemy1AniArr.push_back(enemy1Ani);
				spawnTime = 0;
			}

			for (size_t i = 0; i < enemies.size(); i++)
			{
				enemy1AniArr[i].Update(0, deltaTime, true);
				enemies[i].setTextureRect(enemy1AniArr[i].uvRect);
				enemies[i].move(-1.f, 0.f);

				if ((enemies[i].getPosition().x < 0) || (enemies[i].getPosition().y > 550))
					enemies.erase(enemies.begin() + i);
			}
			
			
			//Make animated rocks
			if (score<20 || health ==0)
				if (spawnRock < spawnMaxRock) spawnRock++;
			if (spawnRock >= spawnMaxRock) {
				rock.setPosition(400 + rand() % 401, 0.f);
				rock.setTexture(&rockTexture);
				rock.setOrigin(1.f, 1.f);
				//rock.setRotation(-45);
				//rock.setScale(1.5f, 1.5f);
				
				rocks.push_back(RectangleShape(rock));
				rockAniArr.push_back(rockAni);
				spawnRock = 0;
			}
			for (int i = 0; i < rocks.size(); i++) {
				rockAniArr[i].Update(0, deltaTime, true);
				rocks[i].setTextureRect(rockAniArr[i].uvRect);
				rocks[i].move(-1.5f, 1.5f);


				if ((rocks[i].getPosition().x < 0) || (rocks[i].getPosition().y > 550))
					rocks.erase(rocks.begin() + i);
			}
			
			//Collision between bullet and enemy
			if (!enemies.empty() && !bullets.empty())
			{
				for (size_t i = 0; i < bullets.size(); i++)
				{
					for (size_t k = 0; k < enemies.size(); k++)
					{
						if (bullets[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
						{
							explosion.setPosition(enemies[k].getPosition().x, enemies[k].getPosition().y);
							explosion.setOrigin(25.f, 25.f);

							explosions.push_back(explosion);

							exploAniArr.push_back(exploAni);

							countFrame = 0;

							bullets.erase(bullets.begin() + i);
							enemies.erase(enemies.begin() + k);

							die.play();

							//cal score
							score++;
							stringstream ss;
							ss << score;
							string temp = ss.str();
							point.setString("Score: " + temp);
							point.setCharacterSize(15);

							if (maxSpawnTime == 30)
								maxSpawnTime = 30;
							else
								maxSpawnTime -= 5;
							
							if (spawnRock==20)
								spawnMaxRock = 20;
							else spawnRock -= 5;

							break;
						}
					}
				}
			}

			//Collision between player and enemy
			for (size_t i = 0; i < enemies.size(); i++)
			{
				if (mask.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
				{
					explosion.setPosition(enemies[i].getPosition().x, enemies[i].getPosition().y);
					collidedX = enemies[i].getPosition().x;
					collidedY = enemies[i].getPosition().y;
					explosion.setOrigin(25.f, 25.f);

					explosions.push_back(explosion);

					exploAniArr.push_back(exploAni);

					countFrame = 0;

					isDrawPlayer = false;
					enemies.erase(enemies.begin() + i);

					die.play();

					//cal life
					lifepoint--;
					stringstream ssLife;
					ssLife << lifepoint;
					string temp1 = ssLife.str();
					life.setString("Life: " + temp1);
					life.setCharacterSize(15);

					break;
				}
			}

			//collision between player and rock
			for (size_t i = 0; i < rocks.size(); i++)
			{
				if (mask.getGlobalBounds().intersects(rocks[i].getGlobalBounds()))
				{
					cout << "collided" << endl;
					explosion.setPosition(rocks[i].getPosition().x, rocks[i].getPosition().y);
					collidedX = rocks[i].getPosition().x;
					collidedY = rocks[i].getPosition().y;
					explosion.setOrigin(25.f, 25.f);

					explosions.push_back(explosion);

					exploAniArr.push_back(exploAni);

					countFrame = 0;

					isDrawPlayer = false;
					rocks.erase(rocks.begin() + i);

					die.play();

					//cal life
					lifepoint--;
					stringstream ssLife;
					ssLife << lifepoint;
					string temp1 = ssLife.str();
					life.setString("Life: " + temp1);
					life.setCharacterSize(15);

					break;
				}
			}

			//Collision between player and rocket
			for (size_t i = 0; i < rockets.size(); i++)
			{
				if (mask.getGlobalBounds().intersects(rockets[i].getGlobalBounds()))
				{
					//cout << "collided" << endl;
					explosion.setPosition(rockets[i].getPosition().x, rockets[i].getPosition().y);
					collidedX = rockets[i].getPosition().x;
					collidedY = rockets[i].getPosition().y;
					explosion.setOrigin(25.f, 25.f);

					explosions.push_back(explosion);

					exploAniArr.push_back(exploAni);

					countFrame = 0;

					isDrawPlayer = false;
					rockets.erase(rockets.begin() + i);

					die.play();

					//cal life
					lifepoint--;
					stringstream ssLife;
					ssLife << lifepoint;
					string temp1 = ssLife.str();
					life.setString("Life: " + temp1);
					life.setCharacterSize(15);

					break;
				}
			}

			//Update for explosion
			for (int i = 0; i < explosions.size(); i++) {
				exploAniArr[i].Update(0, deltaTime, true);

				explosions[i].setTextureRect(exploAniArr[i].uvRect);
			}

			//Draw to screen
			if (lifepoint > 0) {
				window.clear();
				window.draw(background);
				if (isDrawPlayer == true)
					window.draw(player);
				else {
					playerSpawn++;
					if (playerSpawn >= 60)
					{
						player.setPosition(50.f, 300.f);
						isDrawPlayer = true;
						playerSpawn = 0;
					}
				}

				//Control boss
				if ((score>20)||(score>1 && score%20==0)) {
					if (boss.getPosition().x >= 680)
						boss.move(-1.0f, 0.f);

					if (bossShootTime < 100) bossShootTime++;
					else {
						if (boss.getPosition().x == 679)
						{
							rocket.setPosition(500, boss.getPosition().y);
							rockets.push_back(rocket);
						}
						bossShootTime = 0;
					}

					if (bossMoveTime < 200) {
						bossMoveTime++;
						if (boss.getPosition().y<=500)
							boss.move(0.f, direction * 2);
						else boss.move(0.f, -1.f);
					}
					else {
						direction = direction * -1;
						bossMoveTime = 0;
					}
					
					maskBoss.setPosition(boss.getPosition().x, boss.getPosition().y);

					//Collision between bullet and boss
					for (int i = 0; i < bullets.size(); i++) {
						if (bullets[i].getGlobalBounds().intersects(maskBoss.getGlobalBounds()))
						{
							health--;
							cout << health << endl;
							
							explosion.setPosition(bullets[i].getPosition().x, bullets[i].getPosition().y);
							/*collidedX = rockets[i].getPosition().x;
							collidedY = rockets[i].getPosition().y;*/
							explosion.setOrigin(25.f, 25.f);

							explosions.push_back(explosion);

							exploAniArr.push_back(exploAni);

							countFrame = 0;

							//isDrawPlayer = false;
							bullets.erase(bullets.begin() + i);

							die.play();
							
							
							break;
						}
					}

					if (health == 0) boss.move(490, 1.0);

					window.draw(boss);
					for (int i = 0; i < rockets.size(); i++) {
						rockets[i].move(-2.f, 0.f);
					}
					for (int i = 0; i < rockets.size(); i++)
						window.draw(rockets[i]);
				}
				
				//Draw enemies
				for (int i = 0; i < enemies.size(); i++)
				{
					window.draw(enemies[i]);
				}

				//Draw rocks
				for (int i = 0; i < rocks.size(); i++) {
					window.draw(rocks[i]);
				}

				//Draw bullets
				for (int i = 0; i < bullets.size(); i++)
				{
					window.draw(bullets[i]);
				}

				//Explosion
				countFrame++;
				for (int i = 1; i < explosions.size(); i++) {
					window.draw(explosions[i]);
					if (countFrame >= 55)
						explosions.erase(explosions.begin() + i);
				}

				//Draw point
				window.draw(point);
				window.draw(life);

				window.display();
			}
			else {
				music.stop();
				window.clear();
				countBgOver++;
				over.setString("Game over");
				over.setCharacterSize(60);
				over.setPosition(250.f, 250.f);

				esc.setString("Press ESC quit the game");
				esc.setCharacterSize(20);
				esc.setPosition(280.f, 320.f);

				if (countBgOver > 60) {
					window.draw(over);
					window.draw(backOver);
					if (isPlay == true) {
						gameoverSound.play();
						isPlay = false;
					}
				}
				die.stop();
				explosion.setPosition(collidedX, collidedY);
				explosion.setOrigin(25.f, 25.f);

				explosions.push_back(explosion);
				exploAniArr.push_back(exploAni);

				for (int i = 0; i < explosions.size(); i++) {
					exploAniArr[i].Update(0, deltaTime, true);
					explosions[i].setTextureRect(exploAniArr[i].uvRect);
				}

				countFrame++;
				//cout << countFrame << endl;
				for (int i = 1; i < explosions.size(); i++) {
					if ((countFrame < 20) && (isDrawLastEx == true))
					{
						window.draw(explosions[i]);
					}
					explosions.erase(explosions.begin() + i);
				}
				if (countFrame >= 20) isDrawLastEx = false;

				if (countBgOver > 60) {
					window.draw(over);
					if (countBgOver % 100 >= 50)
						window.draw(esc);
				}

				window.display();
			}
		}
		else {
			window.clear();
			window.draw(backOver);
			over.setString("Space invaders");
			over.setPosition(260.f, 260.f);
			over.setCharacterSize(30);

			esc.setString("Press Left Ctrl to play");
			esc.setCharacterSize(10);
			esc.setPosition(330.f, 300.f);

			countFrame++;
			if (countFrame % 100 >= 50)
				window.draw(esc);
			window.draw(over);
			window.display();
		}
	}

	return 0;
}