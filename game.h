/*********************************************************************
 * File: game.h
 * Description: The game of Asteroids. This class holds each piece
 *  of the game (rocks, bullets, ship, score, stars, etc..). It also has
 *  methods that make the game happen (advance, interact, etc.)
 *  This class can not run by it self.
 *********************************************************************/

#ifndef GAME_H
#define GAME_H

#include "uiDraw.h"
#include "uiInteract.h"
#include "point.h"
#include "velocity.h"
#include "bullet.h"
#include "rocks.h"
#include <vector>
#include "ship.h"
#include "levels.h"
#include "stars.h"

 /*****************************************
  * GAME
  * The main game class containing all the state
  *****************************************/
class Game
{
public:
	/*********************************************
	 * Constructor
	 * Initializes the game
	 *********************************************/
	Game(Point tl, Point br);
	~Game();

	/*********************************************
	 * Function: handleInput
	 * Description: Takes actions according to whatever
	 *  keys the user has pressed.
	 *********************************************/
	void handleInput(const Interface & ui);

	/*********************************************
	 * Function: advance
	 * Description: Move everything forward one
	 *  step in time.
	 *********************************************/
	void advance();

	/*********************************************
	 * Function: draw
	 * Description: draws everything for the game.
	 *********************************************/
	void draw(const Interface & ui);

private:
	// The coordinates of the screen
	Point topLeft;
	Point bottomRight;

	static bool gameOver; //used for Game over
	static bool changeHighScore; //show a new high score logo at Game over
	static int numLargeRocks;  //counts how many large rocks were created this level
	static int numMediumRocks; //     "            medium             "
	static int numSmallRocks;  //     "            small              "
	static bool justDied; // This is used for immortality after death
	static bool legalPassword; // This variable keeps a bool of whether password was valid 
	static bool bonusWeapon;  // This variable checks if ship currently has shockwave weapon
	static bool bonusShield;  // This variable checks if ship currently has a shield

	//used to seperate creation of items. 
	static unsigned int frameCount;

	//keeps track of current score
	static unsigned int score;

	//keeps track of highest score in session
	static unsigned int highScore;

	//Will control the current level and lives
	Levels level;

	//Will control all variables related to ship
	Ship ship;

	//Will hold all bullets
	std::vector<Bullet> bullets;

	//Will hold the bullets that represent the background stars
	std::vector<Background> background;

	//Will hold all active asteroids
	std::vector<Rocks *> asteroids;

	//Will hold all active stars
	std::vector<Stars *> stars;


	/*************************************************
	 * Private methods to help with the game logic.
	 *************************************************/
	bool isOnScreen(const Point & point, int radius);
	void advanceBullets();
	void advanceBackground();
	void advanceRocks(const Interface & ui);
	void advanceShip();
	void advanceStars(const Interface & ui);

	/****************************************
	* Private methods to create objects
	***************************/
	Rocks* makeLargeRocks();
	Rocks * makeMediumRocks();
	Rocks * makeSmallRocks();
	Background makeBackground();
	Stars * makeRedStars();
	Stars * makeBlueStars();
	Stars * makeWhiteStars();
	
	//This method makes the stars for practice mode
	void makeStarsPractice();

	//This method will take care of all object collisions
	void handleCollisions(const Interface & ui);

	//This method will clear out all dead objects
	void cleanUpZombies(const Interface & ui);

	//This method will help with high velocity collision detection
	float getClosestDistance(const FlyingObject & obj1, const FlyingObject & obj2) const;
};


#endif /* GAME_H */
