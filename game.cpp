/*********************************************************************
 * File: game.cpp
 * Description: Contains the implementaiton of the game class
 *  methods. Along with static variable declarations.
 *********************************************************************/

#include "game.h"

// These are needed for the getClosestDistance function...
#include <limits>
#include <algorithm>


/******************************************
* Static variables
******************************************/
unsigned int Game::highScore = 0;
bool Game::gameOver = false;
bool Game::changeHighScore = false;
int Game::numLargeRocks = 0;
int Game::numMediumRocks = 0;
int Game::numSmallRocks = 0;
unsigned int Game::frameCount = 0;
bool Game::justDied = false;
unsigned int Game::score = 0;
bool Game::legalPassword = false;
bool Game::bonusShield = false;
bool Game::bonusWeapon = false;


/***************************************
 * GAME CONSTRUCTOR
 ***************************************/
Game::Game(Point tl, Point br)
	: topLeft(tl), bottomRight(br)
{
}

/****************************************
 * GAME DESTRUCTOR
 ****************************************/
Game :: ~Game()
{
	if (!(asteroids.empty()))
	{
		for (int i = 0; i < asteroids.size(); i++)
			delete asteroids[i];
		asteroids.clear();
	}
	if (!(bullets.empty()))
		bullets.clear();

	if (!(background.empty()))
		background.clear();
	if (!(stars.empty()))
	{
		for (int i = 0; i < stars.size(); i++)
			delete stars[i];
		stars.clear();
	}
}

/***************************************
 * GAME :: ADVANCE
 * advance the game one unit of time
 ***************************************/
void Game::advance()
{
	/* Need ui in many advance methods. Would rather have only one instance of it in advance.
	* so created ui for static variables in this method to pass down as reference 
	*/
	Interface ui;
	if (!ui.isPause())
	{
		advanceBackground();
		advanceBullets();
		if (!ui.displayPassword())
		{
			advanceRocks(ui);
			advanceShip();
			advanceStars(ui);
			handleCollisions(ui);
		}
		cleanUpZombies(ui);
		frameCount++;
	}

	if (score > highScore && !ui.isPractice())
	{
		highScore = score;
		changeHighScore = true;
	}

	if (ui.passwordEntered())
	{
		legalPassword = level.usePasskey(ui.getPassword());
	}
}

/***************************************
 * GAME :: ADVANCE BULLETS
 * Go through each bullet and advance it.
 ***************************************/
void Game::advanceBullets()
{
	// Move each of the bullets forward if it is alive
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].isAlive())
		{
			// this bullet is alive, so tell it to move forward
			bullets[i].advance();

			if (!isOnScreen(bullets[i].getPoint(), 2))
			{
				bullets[i].flipSide(abs(bullets[i].getPoint().getY()) >= topLeft.getY(), abs(bullets[i].getPoint().getX()) >= bottomRight.getX());
			}

		}
	}
}

/**********
* Advance Background
**********/
void Game::advanceBackground()
{
	if (background.size() < 50 && (frameCount % 2 == 0))
		background.push_back(makeBackground());

	// Move each the background along
	for (int i = 0; i < background.size(); i++)
	{
		background[i].advance();

		if (!isOnScreen(background[i].getPoint(), 2))
		{
			background[i].flipSide(abs(background[i].getPoint().getY()) >= topLeft.getY(), abs(background[i].getPoint().getX()) >= bottomRight.getX());
		}
	}
}

/**************************************************************************
 * GAME :: ADVANCE ROCKS
 **************************************************************************/
void Game::advanceRocks(const Interface & ui)
{
	if ((ui.isPractice() || ui.isMenu()) && !(ui.isClearScreen()))
	{
		if (asteroids.size() < 5)
			asteroids.push_back(makeLargeRocks());

		for (int i = 0; i < asteroids.size(); i++)
		{
			// we have a asteroid, make sure it's alive
			if (asteroids[i]->isAlive())
			{
				// move it forward
				asteroids[i]->advance();

				// check if the rock has gone off the screen
				if (!isOnScreen(asteroids[i]->getPoint(), asteroids[i]->getRadius()))
				{
					asteroids[i]->flipSide(abs(asteroids[i]->getPoint().getY()) >= topLeft.getY(), abs(asteroids[i]->getPoint().getX()) >= bottomRight.getX());
				}
			}
		}
	}
	
	else if (ui.isLevels() && !(ui.isClearScreen()))
	{
		if (asteroids.size() < 10 || ((int)(frameCount / FRAMES_VARIABLES_MULTIPLIER) % 360) == 0)
		{
			if (numSmallRocks < level.getLevel() % 3)
			{
				numSmallRocks++;
				asteroids.push_back(makeSmallRocks());
			}
			else if (numMediumRocks < (level.getLevel() % 9) / 3)
			{
				numMediumRocks++;
				asteroids.push_back(makeMediumRocks());
			}
			else if (numLargeRocks < level.getLevel() / 9)
			{
				numLargeRocks++;
				asteroids.push_back(makeLargeRocks());
			}
			else if (asteroids.empty())
			{
				level.addLevel();
				numSmallRocks = 0;
				numMediumRocks = 0;
				numLargeRocks = 0;
				frameCount = 0;
				if (level.getLevel() % 10 == 0)
				{
					//Need to temporaryly control input.
					Interface temp;
					temp.setTenthLevel(true);
				}
			}
		}

		for (int i = 0; i < asteroids.size(); i++)
		{
			// we have a asteroid, make sure it's alive
			if (asteroids[i]->isAlive())
			{
				// move it forward
				asteroids[i]->advance();

				// check if the rock has gone off the screen
				if (!isOnScreen(asteroids[i]->getPoint(), asteroids[i]->getRadius()))
				{
					asteroids[i]->flipSide(abs(asteroids[i]->getPoint().getY()) >= topLeft.getY(), abs(asteroids[i]->getPoint().getX()) >= bottomRight.getX());
				}
			}
		}
	
	}
}

/**************************************************************************
* GAME :: ADVANCE SHIP
* Move the ship if ship is alive.
***************************************************************************/
void Game::advanceShip()
{
	//if ship is alive then move the ship
	if (ship.isAlive())
		ship.advance();
	//if ship goes off screen then move to opposite side
	if (!isOnScreen(ship.getPoint(), ship.getRadius()))
		ship.flipSide(abs(ship.getPoint().getY()) >= topLeft.getY(), abs(ship.getPoint().getX()) >= bottomRight.getX());
}

/************
* Advance the stars
*************/
void Game::advanceStars(const Interface & ui)
{
	if (ui.isPractice() && !(ui.isClearScreen()))
	{
		//Should only be called once
		if (stars.empty())
		{
			makeStarsPractice();
		}
	}
	else if (ui.isMenu() && !(ui.isClearScreen()))
	{
		if (frameCount % 400 == 20)
		{
			int type = random(1, 4);
			switch (type)
			{
			case 1:
				stars.push_back(makeRedStars());
				break;
			case 2:
				stars.push_back(makeBlueStars());
				break;
			case 3:
				stars.push_back(makeWhiteStars());
				break;
			}
		}
	}
	//move the stars along
	if (!ui.isPractice())
	{
		for (int i = 0; i < stars.size(); i++)
		{
			if (stars[i]->isAlive())
			{
				// move it forward
				stars[i]->advance();

				// check if the star has gone off the screen
				if (!isOnScreen(stars[i]->getPoint(), stars[i]->getRadius()))
				{
					stars[i]->flipSide(abs(stars[i]->getPoint().getY()) >= topLeft.getY(), abs(stars[i]->getPoint().getX()) >= bottomRight.getX());
				}
			}
		}
	}
}

/**************************************************************************
 * GAME :: Make large Rock
 * Create a large rock and return it.
 **************************************************************************/
Rocks* Game::makeLargeRocks()
{
	int side = random(0, 4);
	Rocks* rock = NULL;
	switch (side)
	{
	case 0:
		rock = new BigRock(Point(topLeft.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 1:
		rock = new BigRock(Point(bottomRight.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 2:
		rock = new BigRock(Point(random(topLeft.getX(), bottomRight.getX()), topLeft.getY()));
		break;
	case 3:
		rock = new BigRock(Point(random(topLeft.getX(), bottomRight.getX()), bottomRight.getY()));
		break;
	}
	return rock;
}

/**************************************************************************
 * GAME :: Make Medium Rock
 * Create a medium rock and return it.
 **************************************************************************/
Rocks* Game::makeMediumRocks()
{
	int side = random(0, 4);
	Rocks* rock = NULL;
	switch (side)
	{
	case 0:
		rock = new MediumRock(Point(topLeft.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 1:
		rock = new MediumRock(Point(bottomRight.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 2:
		rock = new MediumRock(Point(random(topLeft.getX(), bottomRight.getX()), topLeft.getY()));
		break;
	case 3:
		rock = new MediumRock(Point(random(topLeft.getX(), bottomRight.getX()), bottomRight.getY()));
		break;
	}
	return rock;
}

/**************************************************************************
 * GAME :: Make Small Rock
 * Create a small rock and return it.
 **************************************************************************/
Rocks* Game::makeSmallRocks()
{
	int side = random(0, 4);
	Rocks* rock = NULL;
	switch (side)
	{
	case 0:
		rock = new SmallRock(Point(topLeft.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 1:
		rock = new SmallRock(Point(bottomRight.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 2:
		rock = new SmallRock(Point(random(topLeft.getX(), bottomRight.getX()), topLeft.getY()));
		break;
	case 3:
		rock = new SmallRock(Point(random(topLeft.getX(), bottomRight.getX()), bottomRight.getY()));
		break;
	}
	return rock;
}

/***********************************************************************************************
* GAME :: MAKE BACKGROUND
* Will make bullets to fly accross screen to look like stars in distance.
********************************************************************************************/
Background Game::makeBackground()
{
	Point location(random(topLeft.getX(), bottomRight.getX()), random(bottomRight.getY(), topLeft.getY()));
	return Background(location, Velocity( -5.0 / FRAMES_VARIABLES_MULTIPLIER, -5.0 / FRAMES_VARIABLES_MULTIPLIER));
}

/***********
* Make a red star
************/
Stars * Game::makeRedStars()
{
	int side = random(0, 4);
	Stars * star = NULL;
	switch (side)
	{
	case 0:
		star = new RedStar(Point(topLeft.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 1:
		star = new RedStar(Point(bottomRight.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 2:
		star = new RedStar(Point(random(topLeft.getX(), bottomRight.getX()), topLeft.getY()));
		break;
	case 3:
		star = new RedStar(Point(random(topLeft.getX(), bottomRight.getX()), bottomRight.getY()));
		break;
	}
	return star;
}

/**************
* Make a blue star
***************/
Stars * Game::makeBlueStars()
{
	int side = random(0, 4);
	Stars * star = NULL;
	switch (side)
	{
	case 0:
		star = new BlueStar(Point(topLeft.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 1:
		star = new BlueStar(Point(bottomRight.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 2:
		star = new BlueStar(Point(random(topLeft.getX(), bottomRight.getX()), topLeft.getY()));
		break;
	case 3:
		star = new BlueStar(Point(random(topLeft.getX(), bottomRight.getX()), bottomRight.getY()));
		break;
	}
	return star;
}

/**************
* Make a white star
**************/
Stars * Game::makeWhiteStars()
{
	int side = random(0, 4);
	Stars * star = NULL;
	switch (side)
	{
	case 0:
		star = new WhiteStar(Point(topLeft.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 1:
		star = new WhiteStar(Point(bottomRight.getX(), random(bottomRight.getY(), topLeft.getY())));
		break;
	case 2:
		star = new WhiteStar(Point(random(topLeft.getX(), bottomRight.getX()), topLeft.getY()));
		break;
	case 3:
		star = new WhiteStar(Point(random(topLeft.getX(), bottomRight.getX()), bottomRight.getY()));
		break;
	}
	return star;
}

/***************
* Make stationary stars
****************/
void Game::makeStarsPractice()
{
	Stars * star = NULL;
	star = new BlueStar(Point(topLeft.getX() + 120, bottomRight.getY() + 80));
	stars.push_back(star);
	star = new WhiteStar(Point(topLeft.getX() + 240, bottomRight.getY() + 80));
	stars.push_back(star);
}

/**************************************************************************
 * GAME :: IS ON SCREEN
 * Determines if a given point is on the screen.
 **************************************************************************/
bool Game::isOnScreen(const Point & point, int radius)
{
	return (point.getX() >= topLeft.getX() - radius
		&& point.getX() <= bottomRight.getX() + radius
		&& point.getY() >= bottomRight.getY() - radius
		&& point.getY() <= topLeft.getY() + radius);
}

/**************************************************************************
 * GAME :: HANDLE COLLISIONS
 * Check for a collision between an asteroid and a bullet.
 **************************************************************************/
void Game::handleCollisions(const Interface & ui)
{
	static int immortal = 0;
	// now check for a hit (if it is close enough to any live bullets)
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].isAlive())
		{
			// this bullet is alive, see if its too close
			for (int starCount = 0; starCount < stars.size(); starCount++)
			{
				if (stars[starCount]->isAlive())
				{
					if (stars[starCount]->getRadius() >= getClosestDistance(bullets[i] , *(stars[starCount])))
					{
						int bonus = stars[starCount]->getValue();
						switch (bonus)
						{
						case 1:
							level.addLife();
							break;
						case 2:
							bonusWeapon = true;
							break;
						case 3:
							bonusShield = true;
							break;
						}
						if(!ui.isPractice())
							stars[starCount]->kill();
					}
				}
			}

			// check if the asteroid is at this point (in case it was hit)
			for (int asteroidCount = 0; asteroidCount < asteroids.size(); asteroidCount++)
			{
				if (asteroids[asteroidCount]->isAlive())
				{
					if ((asteroids[asteroidCount]->getRadius()) >= getClosestDistance(bullets[i], *(asteroids[asteroidCount])))
					{
						//we have a hit!
						std::vector<Rocks*> temp;
						temp = asteroids[asteroidCount]->killRocks();
						for (int tempCount = 0; tempCount < temp.size(); tempCount++)
						{
							asteroids.push_back(temp[tempCount]);
						}
						// hit the asteroid
						int points = asteroids[asteroidCount]->getValue();
						score += points; 

						// the bullet is dead as well
						bullets[i].kill();

						// When asteroid dies randomly decide to make star
						if ((ui.isLevels()) && !(ui.isClearScreen()))
						{
							if (random(0, (20 + level.getLevel())) <= 10)
							{
								int type = random(1, 4);
								switch (type)
								{
								case 1:
									stars.push_back(makeRedStars());
									break;
								case 2:
									stars.push_back(makeBlueStars());
									break; 
								case 3:
									stars.push_back(makeWhiteStars());
									break;
								}
							}
						}
					}
				}			
			}

		} // if bullet is alive

	} // for bullets

	// now check for collision with ship
	if (ship.isAlive() && !justDied && !ui.isMenu())
	{
		for (int i = 0; i < asteroids.size(); i++)
		{
			if (asteroids[i]->isAlive())
			{
				if ((asteroids[i]->getRadius() + ship.getRadius()) >= getClosestDistance(ship, *(asteroids[i])))
				{
					justDied = true;
					immortal = (30 * FRAMES_VARIABLES_MULTIPLIER * 3);
					if (!bonusShield)
					{
						ship.kill();
					}
					else
					{
						asteroids[i]->kill();
						bonusShield = false;
					}
				}
			}
		}
		for (int i = 0; i < stars.size(); i++)
		{
			if (stars[i]->isAlive())
			{
				if ((stars[i]->getRadius() + ship.getRadius()) >= getClosestDistance(ship, *(stars[i])))
				{
					int bonus = stars[i]->getValue();
					switch (bonus)
					{
					case 1:
						level.addLife();
						break;
					case 2:
						bonusWeapon = true;
						break;
					case 3:
						bonusShield = true;
						break;
					}
					if (!ui.isPractice())
						stars[i]->kill();
				}
			}
		}
	}
	else if (justDied)
	{
		if ((--immortal) == 0 || frameCount == 0)
		{
			justDied = false;
		}
	}
}

/**************************************************************************
 * GAME :: CLEAN UP ZOMBIES
 * Remove any dead objects (take bullets out of the list, deallocate rocks)
 **************************************************************************/
void Game::cleanUpZombies(const Interface & ui)
{
	// check for dead asteroid
	std::vector<Rocks*>::iterator asteroidIt = asteroids.begin();
	while (asteroidIt != asteroids.end())
	{
		Rocks* pAsteroid = *asteroidIt;

		if (!(pAsteroid->isAlive()) || ui.isClearScreen())
		{
			delete pAsteroid;
			// remove from list and advance
			asteroidIt = asteroids.erase(asteroidIt);
		}
		else
		{
			asteroidIt++; // advance
		}
	}

	// Look for dead bullets
	std::vector<Bullet>::iterator bulletIt = bullets.begin();
	while (bulletIt != bullets.end())
	{
		Bullet bullet = *bulletIt;

		if ((!bullet.isAlive()) || ui.isClearScreen())
		{
			// remove from list and advance
			bulletIt = bullets.erase(bulletIt);
		}
		else
		{
			bulletIt++; // advance
		}
	}

	std::vector<Background>::iterator backgroundIt = background.begin();
	while (backgroundIt != background.end())
	{
		Background temp = *backgroundIt;

		if ((!temp.isAlive()) || ui.isClearScreen())
		{
			// remove from list and advance
			backgroundIt = background.erase(backgroundIt);
		}
		else
		{
			backgroundIt++; // advance
		}
	}

	if (!ship.isAlive() && !gameOver)
	{
		if (ui.isPractice())
		{
			ship.reset();
			score = 0;
		}
		else
		{
			if (level.getLives() > 1)
			{
				ship.reset();
				level.takeLife();
			}
			else
			{
				gameOver = true;
				level.takeLife();
			}
		}
	}

	// check for dead star
	std::vector<Stars*>::iterator starsIt = stars.begin();
	while (starsIt != stars.end())
	{
		Stars * pStar = *starsIt;

		if (!(pStar->isAlive()) || ui.isClearScreen())
		{
			delete pStar;
			// remove from list and advance
			starsIt = stars.erase(starsIt);
		}
		else
		{
			starsIt++; // advance
		}
	}
}

/***************************************
 * GAME :: HANDLE INPUT
 * accept input from the user
 ***************************************/
void Game::handleInput(const Interface & ui)
{
	//need static variables.
	if (!(ui.isMenu()) && !(gameOver) && !(ui.isPause())) //don't interact if on menu
	{
		// Change the direction of the rifle
		if (ui.isLeft())
		{
			//rotates ship left
			ship.rotate(true);
		}

		if (ui.isRight())
		{
			//rotates ship right
			ship.rotate(false);
		}

		if (ui.isUp())
		{
			//addes thrust to ship
			ship.thrust();
		}

		if (ui.isDown())
		{
			//addes reverse thrust
			ship.reverseThruster();
		}

		if (ui.isStrafeL())
		{
			ship.strafe(true);
		}

		if (ui.isStrafeR())
		{
			ship.strafe(false);
		}

		// Check for "Spacebar
		if (ui.isSpace())
		{
			Bullet newBullet(ship.getPoint(), ship.getAngle(), ship.getVelocity());
			bullets.push_back(newBullet);
		}

		if (ui.shockWave())
		{
			if (bonusWeapon)
			{
				for (int i = 0; i < asteroids.size(); i++)
				{
					score += asteroids[i]->hit();
					delete asteroids[i];
				}
				asteroids.clear();
				bonusWeapon = false;
			}
		}

	}
	else if (gameOver)
	{
		if (ui.isMenu())
		{
			gameOver = false;
			level.reset();
			numSmallRocks = 0;
			numMediumRocks = 0;
			numLargeRocks = 0;
			frameCount = 0;
			score = 0;
			justDied = false;
			ship.reset();
			bonusShield = false;
			bonusWeapon = false;
		}
	}

	else if (ui.isMenu())
	{
		if ((ui.isPractice()) || (ui.isLevels()))
		{
			//Want to keep const modifier on ui. Making a temp interface
			//to change menu. Since menu is a static variable. This will work.
			Interface temp;
			temp.setMenu(false);
			changeHighScore = false;
			ship.reset();
			numSmallRocks = 0;
			numMediumRocks = 0;
			numLargeRocks = 0;
			frameCount = 0;
			bonusShield = false;
			bonusWeapon = false;
			if (ui.isNewStart())
			{
				level.reset();
				score = 0;
			}
		}
		else if (ui.passwordEntered())
		{
			numSmallRocks = 0;
			numMediumRocks = 0;
			numLargeRocks = 0;
			frameCount = 0;
			ship.reset();
			bonusShield = false;
			bonusWeapon = false;
		}
		else if (ui.isQuit())
			exit(0);
	}
}

/*********************************************
 * GAME :: DRAW
 * Draw everything on the screen
 *********************************************/
void Game::draw(const Interface & ui)
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		if (asteroids[i]->isAlive())
		{
			asteroids[i]->draw();
		}
	}

	// draw the bullets, if they are alive
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i].isAlive())
		{
			bullets[i].draw();
		}
	}

	// draw background
	for (int i = 0; i < background.size(); i++)
	{
		if (background[i].isAlive())
		{
			background[i].draw();
		}
	}

	// draw the stars
	for (int i = 0; i < stars.size(); i++)
	{
		if (stars[i]->isAlive())
		{
			stars[i]->draw();
		}
	}

	static int flashCount = 0; //Flash when just died
   	if (!ui.isMenu() && !gameOver && !ui.displayPassword())
	{
		if (!justDied)
		{
			ship.draw();
			if (bonusShield)
			{
				drawCircle(ship.getPoint(), ship.getRadius() + 2);
			}
		}
		else
		{
			flashCount++;
			if (flashCount < 7 * FRAMES_VARIABLES_MULTIPLIER)
			{
				ship.draw();
			}
			else if (flashCount >= 14 * FRAMES_VARIABLES_MULTIPLIER)
				flashCount = 0;
		}
	}
	else if (ui.displayPassword())
	{
		displayPassword(Point(0, 0), level.getPasskey());
	}

	// Put the score on the screen
	drawScore(Point(topLeft.getX() + 20, topLeft.getY() - 30), score);
	drawHighScore(Point(-45, topLeft.getY() - 30), highScore);
	if (bonusWeapon)
		drawWeaponAvailable(Point(-120, bottomRight.getY() + 40));

	if (ui.isFramesPerSecond())
		drawNumber(Point(bottomRight.getX() - 60, topLeft.getY() - 40), ui.getFramesPerSecond());
	
	if (ui.isLevels())
	{
		drawLevel(Point(bottomRight.getX() - 70, bottomRight.getY() + 30), level.getLevel());
		drawLives(Point(topLeft.getX() + 20, bottomRight.getY() + 30), level.getLives());
	}
	else if (ui.isMenu())
	{
		if (ui.isHowToPlay())
		{
			drawHowTo(Point(-180, 160));
		}
		else if (ui.isEnterPassword())
		{
			drawPassword(Point(0, 0), ui.getPassword());
		}
		else if (ui.passwordEntered())
		{
			drawPasswordResult(Point(0, 0), legalPassword);
		}
		else
		{
			drawMenu(Point(-100, 50));
		}

	}
	if (gameOver)
	{
		drawGameOver(Point(-60, 90), highScore, changeHighScore);
	}
	else if (ui.isPause())
	{
		drawPause(Point(-35, 0));
	}
}

/**********************************************************
 * Function: getClosestDistance
 * Description: Determine how close these two objects will
 *   get in between the frames.
 **********************************************************/

float Game :: getClosestDistance(const FlyingObject &obj1, const FlyingObject &obj2) const
{
   // find the maximum distance traveled
   float dMax = std::max(abs(obj1.getVelocity().getDx()), abs(obj1.getVelocity().getDy()));
   dMax = std::max(dMax, abs(obj2.getVelocity().getDx()));
   dMax = std::max(dMax, abs(obj2.getVelocity().getDy()));
   dMax = std::max(dMax, 0.1f); // when dx and dy are 0.0. Go through the loop once.
   
   float distMin = std::numeric_limits<float>::max();
   for (float i = 0.0; i <= dMax; i++)
   {
      Point point1(obj1.getPoint().getX() + (obj1.getVelocity().getDx() * i / dMax),
                     obj1.getPoint().getY() + (obj1.getVelocity().getDy() * i / dMax));
      Point point2(obj2.getPoint().getX() + (obj2.getVelocity().getDx() * i / dMax),
                     obj2.getPoint().getY() + (obj2.getVelocity().getDy() * i / dMax));
      
      float xDiff = point1.getX() - point2.getX();
      float yDiff = point1.getY() - point2.getY();
      
      float distSquared = (xDiff * xDiff) +(yDiff * yDiff);
      
      distMin = std::min(distMin, distSquared);
   }
   
   return sqrt(distMin);
}