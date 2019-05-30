/*****************************************************
 * File: Driver.cpp
 * Author: Br. Burton
 *
 * Description: This file contains the main function
 *  that starts the game and the callback function
 *  that specifies what methods of the game class are
 *  called each time through the game loop.
 *
 * Timothy Johnson:
 *	I adjusted the size of window in this source file.
 ******************************************************/
#include "game.h"
#include "uiInteract.h"

//Added this global veriable to easy change screen size
constexpr int SIZE_OF_SCREEN = 400;

/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(const Interface *pUI, void *p)
{
   Game *pGame = (Game *)p;
   
   pGame->advance();
   pGame->handleInput(*pUI);
   pGame->draw(*pUI);
}


/*********************************
 * Main is pretty sparse.  Just initialize
 * the game and call the display engine.
 * That is all!
 *********************************/
int main(int argc, char ** argv)
{
	Point topLeft(-(SIZE_OF_SCREEN), SIZE_OF_SCREEN);      //adjusted size from -200, 200 to -400, 400
	Point bottomRight(SIZE_OF_SCREEN, -(SIZE_OF_SCREEN));

   Interface ui(argc, argv, "Asteroids", topLeft, bottomRight);
   Game game(topLeft, bottomRight);
   ui.run(callBack, &game);
   
   return 0;
}
