/***********************************************************************
 * Source File:
 *     UI INTERACT
 * Author:
 *     Br. Helfrich
 * Description:
 *     Implement the interfaces specified in uiInterface.h.  This handles
 *     all the interfaces and events necessary to work with OpenGL.  Your
 *     program will interface with this thorough the callback function
 *     pointer towards the bottom of the file.
 ************************************************************************/

#include <string>     // need you ask?
#include <sstream>    // convert an integer into text
#include <cassert>    // I feel the need... the need for asserts
#include <time.h>     // for clock
#include <cstdlib>    // for rand()
#include <chrono>     // used for sleep
#include <thread>     // "      "


#ifdef __APPLE__
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library

constexpr float FRAMES_CLOCK_MULTIPLIER = 2.0;  //adjust this to make 60 frames.
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>    // Main OpenGL library
#include <GL/glut.h>  // Second OpenGL library

constexpr float FRAMES_CLOCK_MULTIPLIER = 2.0;
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <Gl/glut.h>           // OpenGL library we copied
#include <ctime>            // for ::Sleep();
#include <Windows.h>

constexpr float FRAMES_CLOCK_MULTIPLIER = 1.03;

#define _USE_MATH_DEFINES
#include <math.h>
#endif // _WIN32

#include "uiInteract.h"
#include "point.h"


/************************************************************************
 * DRAW CALLBACK
 * This is the main callback from OpenGL. It gets called constantly by
 * the graphics engine to refresh and draw the window.  Here we will
 * clear the background buffer, draw on it, and send it to the forefront
 * when the appropriate time period has passsed.
 *
 * Note: This and all other callbacks can't be member functions, they must
 * have global scope for OpenGL to see them.
 *************************************************************************/
void drawCallback()
{
   // even though this is a local variable, all the members are static
   Interface ui;
   // Prepare the background buffer for drawing
   glClear(GL_COLOR_BUFFER_BIT); //clear the screen
   glColor3f(1,1,1);
   
   //calls the client's display function
   assert(ui.callBack != NULL);
   ui.callBack(&ui, ui.p);
   
   static int fps = 0;
   static unsigned int nextSecond = clock() + CLOCKS_PER_SEC;
   if (nextSecond <= clock())
   {
	   ui.setFrameCount(fps);
	   fps = 0;
	   nextSecond = clock() + CLOCKS_PER_SEC;
   }
   fps++;

   PointInTime currentTime = std::chrono::high_resolution_clock::now();
   std::chrono::duration< double > timeRemaining = ui.nextDrawTime() - currentTime;
   if (timeRemaining > std::chrono::duration< double >::zero())
	   std::this_thread::sleep_for(timeRemaining);

   // from this point, set the next draw time
   ui.setNextDrawTime(currentTime);

   // bring forth the background buffer
   glutSwapBuffers();

   // clear the space at the end
   ui.keyEvent();
}

/************************************************************************
 * KEY DOWN CALLBACK
 * When a key on the keyboard has been pressed, we need to pass that
 * on to the client.  Currently, we are only registering the arrow keys
 *   INPUT   key:   the key we pressed according to the GLUT_KEY_ prefix
 *           x y:   the position in the window, which we ignore
 *************************************************************************/
void keyDownCallback(int key, int x, int y)
{
   // Even though this is a local variable, all the members are static
   // so we are actually getting the same version as in the constructor.
   Interface ui;
   ui.keyEvent(key, true /*fDown*/);
}

/************************************************************************
 * KEY UP CALLBACK
 * When the user has released the key, we need to reset the pressed flag
 *   INPUT   key:   the key we pressed according to the GLUT_KEY_ prefix
 *           x y:   the position in the window, which we ignore
 *************************************************************************/
void keyUpCallback(int key, int x, int y)
{
   // Even though this is a local variable, all the members are static
   // so we are actually getting the same version as in the constructor.
   Interface ui;
   ui.keyEvent(key, false /*fDown*/);
}

/***************************************************************
 * KEYBOARD CALLBACK
 * Generic callback to a regular ascii keyboard event, such as
 * the space bar or the letter 'q'
 ***************************************************************/
void keyboardCallback(unsigned char key, int x, int y)
{
   // Even though this is a local variable, all the members are static
   // so we are actually getting the same version as in the constructor.
   Interface ui;
   ui.keyEventKeyboard(key, true /*fDown*/);
}

/************
* Handles keyboard keys when let go.
*******/
void keyboardUpCallback(unsigned char key, int x, int y)
{
	Interface ui;
	ui.KeyEventUpKeyboard(key, false);
}

void Interface::KeyEventUpKeyboard(int key, bool fDown)
{
	switch (key)
	{
	case 'S':
	case 's':
		isDownPress = fDown;
		break;
	case 'W':
	case 'w':
		isUpPress = fDown;
		break;
	case 'D':
	case 'd':
		isRightPress = fDown;
		break;
	case 'A':
	case 'a':
		isLeftPress = fDown;
		break;
	case 'Q':
	case 'q':
		isStrafeLPress = fDown;
		break;
	case 'E':
	case 'e':
		isStrafeRPress = fDown;
		break;
	case 'F':
	case 'f':
		isShockWave = fDown;
		break;
	}
}

/**************
* Handles when a key is pressed on keyboard
**********/
void Interface::keyEventKeyboard(int key, bool fDown)
{
	if (!isMenuPress)
	{
		if (isTenthLevel)
		{
			if (key == 'c' || key == 'C')
			{
				isTenthLevel = false;
			}
		}
		else
		{
			switch (key)
			{
			case 'S':
			case 's':
				isDownPress = fDown;
				break;
			case 'W':
			case 'w':
				isUpPress = fDown;
				break;
			case 'D':
			case 'd':
				isRightPress = fDown;
				break;
			case 'A':
			case 'a':
				isLeftPress = fDown;
				break;
			case 'Q':
			case 'q':
				isStrafeLPress = fDown;
				break;
			case 'E':
			case 'e':
				isStrafeRPress = fDown;
				break;
			case ' ':
				isSpacePress = fDown;
				break;
			case 'F':
			case 'f':
				isShockWave = fDown;
				break;
			case 'M':
			case 'm':
			case '\x1B':
				isLevelsMode = false;
				isPracticeMode = false;
				isMenuPress = fDown;
				isPausePress = false;
				break;
			case 'X':
			case 'x':
				wantsFramesPerSecond = !wantsFramesPerSecond;
				break;
			case 'P':
			case 'p':
				isPausePress = !isPausePress;
				break;
			}
		}
	}
	else if (enteringPassword)
	{
		switch (key)
		{
		case '\x0D':
		case ' ':
			enteringPassword = false;
			passwordEnter = true;
			break;
		case '\x08':
			if (password.size() > 0)
				password.erase((password.size() - 1));
			break;

		default:
			password += key;
		}
	}
	else if (passwordEnter)
	{
		switch (key)
		{
		case 'R':
		case 'r':
			enteringPassword = true;
			passwordEnter = false;
			break;
		case 'M':
		case 'm':
			passwordEnter = false;
			break;

		default:
			passwordEnter = false;
			isMenuPress = false;
			isLevelsMode = true;
			clearMenuAnimation = true;
			break;
		}
	}
	else if (isMenuPress)
	{
		switch (key)
		{
		case 'P':
		case 'p':
			clearMenuAnimation = true;
			isPracticeMode = fDown;
			break;
		case 'S':
		case 's':
			clearMenuAnimation = true;
			isLevelsMode = fDown;
			isStartNew = fDown;
			break;
		case 'Q':
		case 'q':
			isQuitPress = fDown;
			break;
		case 'H':
		case 'h':
			wantsControls = !wantsControls;
			break;
		case 'Z':
		case 'z':
			enteringPassword = fDown;
			password.clear();
			break;
		case 'X':
		case 'x':
			wantsFramesPerSecond = !wantsFramesPerSecond;
			break;
		case 'C':
		case 'c':
			isLevelsMode = fDown;
			clearMenuAnimation = true;
			break;
		}
	}
}

/***************************************************************
 * INTERFACE : KEY EVENT
 * Either set the up or down event for a given key
 *   INPUT   key     which key is pressed
 *           fDown   down or brown
 ****************************************************************/
void Interface::keyEvent(int key, bool fDown)
{
	switch (key)
	{
	case GLUT_KEY_DOWN:
		isDownPress = fDown;
		break;
	case GLUT_KEY_UP:
		isUpPress = fDown;
		break;
	case GLUT_KEY_RIGHT:
		isRightPress = fDown;
		break;
	case GLUT_KEY_LEFT:
		isLeftPress = fDown;
		break;
	}
}
/***************************************************************
 * INTERFACE : KEY EVENT
 * Either set the up or down event for a given key
 *   INPUT   key     which key is pressed
 *           fDown   down or brown
 ****************************************************************/
void Interface::keyEvent()
{
   isSpacePress = false;
   clearMenuAnimation = false;
   isStartNew = false;
}

/************************************************************************
 * INTERFACE : SET NEXT DRAW TIME
 * What time should we draw the buffer again?  This is a function of
 * the current time and the frames per second.
 *************************************************************************/
void Interface::setNextDrawTime(PointInTime currentTime)
{
	nextTime = currentTime + timePeriod;
}

/************************************************************************
 * INTERFACE : SET FRAMES PER SECOND
 * The frames per second dictates the speed of the game.  The more frames
 * per second, the quicker the game will appear to the user.  We will default
 * to 30 frames/second but the client can set this at will.
 *    INPUT  value        The number of frames per second.  30 is default
 *************************************************************************/
void Interface::setFramesPerSecond(double value)
{
	timePeriod = std::chrono::seconds(1) / value;
}

/***************************************************
 * STATICS
 * All the static member variables need to be initialized
 * Somewhere globally.  This is a good spot
 **************************************************/
int          Interface::isStrafeLPress = false;
int          Interface::isStrafeRPress = false;
int          Interface::isDownPress  = 0;
int          Interface::isUpPress    = 0;
int          Interface::isLeftPress  = 0;
int          Interface::isRightPress = 0;
bool         Interface::isSpacePress = false;
bool         Interface::initialized  = false;
TimeDuration Interface::timePeriod = std::chrono::seconds(1) / (30 * FRAMES_CLOCK_MULTIPLIER); // default to 30 frames/second
PointInTime  Interface::nextTime = std::chrono::high_resolution_clock::now();        // redraw now please
void *       Interface::p            = NULL;
void (*Interface::callBack)(const Interface *, void *) = NULL;
bool         Interface::isPracticeMode = false;
bool         Interface::isLevelsMode = false;
bool         Interface::isMenuPress = true;   // Game starts in menu
bool         Interface::isQuitPress = false;
bool         Interface::wantsControls = false;
bool         Interface::enteringPassword = false;
bool         Interface::clearMenuAnimation = false;
bool         Interface::wantsFramesPerSecond = false;
int          Interface::framesPerSecond = 0;
bool         Interface::isStartNew = false;
bool         Interface::isPausePress = false;
bool         Interface::passwordEnter = false;
std::string  Interface::password = "";
bool         Interface::isTenthLevel = false;
bool         Interface::isShockWave = false;


/************************************************************************
 * INTERFACE : DESTRUCTOR
 * Nothing here!
 ***********************************************************************/
Interface::~Interface()
{
}


/************************************************************************
 * INTEFACE : INITIALIZE
 * Initialize our drawing window.  This will set the size and position,
 * get ready for drawing, set up the colors, and everything else ready to
 * draw the window.  All these are part of initializing Open GL.
 *  INPUT    argc:       Count of command-line arguments from main
 *           argv:       The actual command-line parameters
 *           title:      The text for the titlebar of the window
 *************************************************************************/
void Interface::initialize(int argc, char ** argv, const char * title, Point topLeft, Point bottomRight)
{
   if (initialized)
      return;
   
   // set up the random number generator
   srand((unsigned int)time(NULL));

   // create the window
   glutInit(&argc, argv);
   Point point;
   glutInitWindowSize(   // size of the window
      (int)(bottomRight.getX() - topLeft.getX()),
      (int)(topLeft.getY() - bottomRight.getY()));
            
   glutInitWindowPosition( 10, 10);                // initial position 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // double buffering
   glutCreateWindow(title);              // text on titlebar
   glutIgnoreKeyRepeat(true);
   
   // set up the drawing style: B/W and 2D
   glClearColor(0, 0, 0, 0);          // Black is the background color
   gluOrtho2D((int)topLeft.getX(), (int)bottomRight.getX(),
              (int)bottomRight.getY(), (int)topLeft.getY()); // 2D environment

   // register the callbacks so OpenGL knows how to call us
   glutDisplayFunc(   drawCallback    );
   glutIdleFunc(      drawCallback    );
   glutKeyboardFunc(  keyboardCallback);
   glutKeyboardUpFunc(keyboardUpCallback);
   glutSpecialFunc(   keyDownCallback );
   glutSpecialUpFunc( keyUpCallback   );
   initialized = true;
   
   // done
   return;
}

/************************************************************************
 * INTERFACE : RUN
 *            Start the main graphics loop and play the game
 * INPUT callBack:   Callback function.  Every time we are beginning
 *                   to draw a new frame, we first callback to the client
 *                   to see if he wants to do anything, such as move
 *                   the game pieces or respond to input
 *       p:          Void point to whatever the caller wants.  You
 *                   will need to cast this back to your own data
 *                   type before using it.
 *************************************************************************/
void Interface::run(void (*callBack)(const Interface *, void *), void *p)
{
   // setup the callbacks
   this->p = p;
   this->callBack = callBack;

   glutMainLoop();

   return;
}

