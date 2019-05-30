/*********************************************
 * Header file:
 *    UI INTERFACE
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This module will create an OpenGL window,
 *    enter the OpenGL main loop, and accept events.
 *    The main methods are:
 *    1. Constructors - Create the window
 *    2. run()        - Run the main loop
 *    3. callback     - Specified in Run, this user-provided
 *                      function will get called with every frame
 *    4. isDown()     - Is a given key pressed on this loop?
 *
 * Timothy Johnson:
 *    A majority of this class has been changed for my benefit.
 *    Added static variables, methods, and input recognition.
 *    Also added a FPS counter that is handled in callback.
 **********************************************/

#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#include "point.h"

#include <chrono>
#include <string>

//This variable had to go in the header file to be used in all files.
//It's base 1.0 is 30, 2.0 is 60...etc This does not change frames, use this to match your frames
//with all advance features.
constexpr float FRAMES_VARIABLES_MULTIPLIER = 2.0;

/*****************
* BROTHER DUDLEY provided this code for use in project
****************/
using TimeDuration = std::chrono::duration<double>;
using PointInTime = std::chrono::time_point<std::chrono::high_resolution_clock, TimeDuration>;

/********************************************
 * INTERFACE
 * All the data necessary to keep our graphics
 * state in memory
 ********************************************/
class Interface
{
public:
   // Default constructor useful for setting up the random variables
   // or for opening the file for output
   Interface() { initialize(0, 0x0000, "Window", Point(-50, 50), Point(50, -50)); };

   // Constructor if you want to set up the window with anything but
   // the default parameters
   Interface(int argc, char ** argv, const char * title, Point topLeft, Point bottomRight)
   {
      initialize(argc, argv, title, topLeft, bottomRight);
   }
   
   // Destructor, incase any housecleaning needs to occr
   ~Interface();

   // This will set the game in motion
   void run(void (*callBack)(const Interface *, void *), void *p);

   // Set the next draw time based on current time and time period
   void setNextDrawTime( PointInTime timePoint );

   // Retrieve the next tick time... the time of the next draw.
   PointInTime nextDrawTime() { return nextTime; };

   // How many frames per second are we configured for?
   void setFramesPerSecond(double value);
   
   // Added to be able to cancel wasd keys when pressed
   void KeyEventUpKeyboard(int key, bool fDown);

   // Added to be able to see keyboard events distinguished from arrows
   void keyEventKeyboard(int key, bool fDown);

   // Key event indicating a key has been pressed or not.  The callbacks
   // should be the only onces to call this
   void keyEvent(int key, bool fDown);
   void keyEvent();

   // Current frame rate
   double frameRate() const { return std::chrono::seconds(1) / timePeriod;   };
   
   // Get various key events
   bool isClearScreen() const { return clearMenuAnimation; };
   bool isHowToPlay() const { return wantsControls; };
   bool isEnterPassword() const { return enteringPassword; };
   bool isFramesPerSecond() const { return wantsFramesPerSecond; };
   int isStrafeL()    const { return isStrafeLPress; };
   int isStrafeR()    const { return isStrafeRPress; };
   int getFramesPerSecond() const { return framesPerSecond; };

   bool isPractice()      const { return isPracticeMode; };
   bool isLevels()      const { return isLevelsMode; };
   bool isMenu()      const { return isMenuPress; };
   bool isQuit()      const { return isQuitPress; };

   int  isDown()      const { return isDownPress;  };
   int  isUp()        const { return isUpPress;    };
   int  isLeft()      const { return isLeftPress;  };
   int  isRight()     const { return isRightPress; };
   bool isSpace()     const { return isSpacePress; };
   bool isNewStart()   const { return isStartNew; }
   bool isPause()     const { return isPausePress; }
   bool passwordEntered() const { return passwordEnter; }
   bool displayPassword() const { return isTenthLevel; }
   bool shockWave() const { return isShockWave; }

   std::string getPassword() const { return password; }
   
   static void *p;                   // for client
   static void (*callBack)(const Interface *, void *);

   // Used to exit menu
   void setMenu(const bool & menu) { isMenuPress = menu; };
   void setFrameCount(const int & fps)
   {
	   framesPerSecond = fps;
   }
   void setTenthLevel(const bool & displayPass) { isTenthLevel = displayPass; }

private:
   void initialize(int argc, char ** argv, const char * title, Point topLeft, Point bottomRight);

   static bool         initialized;  // only run the constructor once!
   static TimeDuration timePeriod;   // interval between frame draws
   static PointInTime nextTime;     // time of our next draw
   static int framesPerSecond;      // used to hold fps 
   static bool wantsFramesPerSecond;  //used to display fps
   static bool isTenthLevel;         // used to control input when displaying password

   static bool isPracticeMode;       // was practice mode selected in menu
   static bool isLevelsMode;         //  "  Levels          "
   static bool isStartNew;           // used when new game is choosen
   static bool isPausePress;         // used when pause is pushed

   static int  isStrafeLPress;       // is the Q button pressed while playing
   static int  isStrafeRPress;       //    "   E           "

   static int  isDownPress;          // is the down arrow currently pressed?
   static int  isUpPress;            //    "   up         "
   static int  isLeftPress;          //    "   left       "
   static int  isRightPress;         //    "   right      "
   static bool isSpacePress;         //    "   space      "

   static bool isMenuPress;          // Was option to return to menu choosen
   static bool isQuitPress;          //  "            quit         "
   static bool wantsControls;        // User wants to see controls
   static bool enteringPassword;     // used when currently entering password
   static bool clearMenuAnimation;   // used to clear screen
   static bool passwordEnter;        // used when a password has been entered
   static bool isShockWave;          // used to know when shock wave is to be used

   static std::string password;      // used for password entry;
};



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
void drawCallback();

/************************************************************************
 * KEY DOWN CALLBACK
 * When a key on the keyboard has been pressed, we need to pass that
 * on to the client.  Currnetly, we are only registering the arrow keys
 *************************************************************************/
void keyDownCallback(int key, int x, int y);

/************************************************************************
 * KEY UP CALLBACK
 * When the user has released the key, we need to reset the pressed flag
 *************************************************************************/
void keyUpCallback(int key, int x, int y);

/***************************************************************
 * KEYBOARD CALLBACK
 * Generic callback to a regular ascii keyboard event, such as
 * the space bar or the letter 'q'
 ***************************************************************/
void keyboardCallback(unsigned char key, int x, int y);

/************************************************************************
 * RUN
 * Set the game in action.  We will get control back in our drawCallback
 *************************************************************************/
void run();

#endif // UI_INTERFACE_H
