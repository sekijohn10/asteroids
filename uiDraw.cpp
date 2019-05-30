/***********************************************************************
 * Source File:
 *    User Interface Draw : put pixels on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This is the code necessary to draw on the screen. We have a collection
 *    of procedural functions here because each draw function does not
 *    retain state. In other words, they are verbs (functions), not nouns
 *    (variables) or a mixture (objects)
 ************************************************************************/

#include <string>     // need you ask?
#include <sstream>    // convert an integer into text
#include <cassert>    // I feel the need... the need for asserts
#include <time.h>     // for clock


#ifdef __APPLE__
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>         // OpenGL library we copied 
#define _USE_MATH_DEFINES
#include <math.h>
#endif // _WIN32

#include "point.h"
#include "uiDraw.h"
#include "uiInteract.h"


#define deg2rad(value) ((M_PI / 180) * (value))

/*********************************************
 * NUMBER OUTLINES
 * We are drawing the text for score and things
 * like that by hand to make it look "old school."
 * These are how we render each individual charactger.
 * Note how -1 indicates "done".  These are paired
 * coordinates where the even are the x and the odd
 * are the y and every 2 pairs represents a point
 ********************************************/
const char NUMBER_OUTLINES[10][20] =
{
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   0,10,  0, 0,  -1,-1, -1,-1},//0
  {7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//1
  {0, 0,  7, 0,   7, 0,  7, 5,   7, 5,  0, 5,   0, 5,  0,10,   0,10,  7,10},//2
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   4, 5,  7, 5,  -1,-1, -1,-1},//3
  {0, 0,  0, 5,   0, 5,  7, 5,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1},//4
  {7, 0,  0, 0,   0, 0,  0, 5,   0, 5,  7, 5,   7, 5,  7,10,   7,10,  0,10},//5
  {7, 0,  0, 0,   0, 0,  0,10,   0,10,  7,10,   7,10,  7, 5,   7, 5,  0, 5},//6
  {0, 0,  7, 0,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//7
  {0, 0,  7, 0,   0, 5,  7, 5,   0,10,  7,10,   0, 0,  0,10,   7, 0,  7,10},//8
  {0, 0,  7, 0,   7, 0,  7,10,   0, 0,  0, 5,   0, 5,  7, 5,  -1,-1, -1,-1} //9
};

/************************************************************************
 * DRAW DIGIT
 * Draw a single digit in the old school line drawing style.  The
 * size of the glyph is 8x11 or x+(0..7), y+(0..10)
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawDigit(const Point & topLeft, char digit)
{
   // we better be only drawing digits
   assert(isdigit(digit));
   if (!isdigit(digit))
      return;

   // compute the row as specified by the digit
   int r = digit - '0';
   assert(r >= 0 && r <= 9);

   // go through each segment.
   for (int c = 0; c < 20 && NUMBER_OUTLINES[r][c] != -1; c += 4)
   {
      assert(NUMBER_OUTLINES[r][c    ] != -1 &&
             NUMBER_OUTLINES[r][c + 1] != -1 &&
             NUMBER_OUTLINES[r][c + 2] != -1 &&
             NUMBER_OUTLINES[r][c + 3] != -1);

      //Draw a line based off of the num structure for each number
      Point start;
      start.setX(topLeft.getX() + NUMBER_OUTLINES[r][c]);
      start.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 1]);
      Point end;
      end.setX(topLeft.getX() + NUMBER_OUTLINES[r][c + 2]);
      end.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 3]);
         
      drawLine(start, end);
   }
}

/*************************************************************************
 * DRAW NUMBER
 * Display an integer on the screen using the 7-segment method
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawNumber(const Point & topLeft, int number)
{
   // our cursor, if you will. It will advance as we output digits
   Point point = topLeft;
   
   // is this negative
   bool isNegative = (number < 0);
   number *= (isNegative ? -1 : 1);
   
   // render the number as text
   std::ostringstream sout;
   sout << number;
   string text = sout.str();

   // handle the negative
   if (isNegative)
   {
      glBegin(GL_LINES);
      glVertex2f(point.getX() + 1, point.getY() - 5);
      glVertex2f(point.getX() + 5, point.getY() - 5);
      glEnd();
      point.addX(11);
   }
   
   // walk through the text one digit at a time
   for (const char *p = text.c_str(); *p; p++)
   {
      assert(isdigit(*p));
      drawDigit(point, *p);
      point.addX(11);
   }
}


/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 *   INPUT  topLeft   The top left corner of the text
 *          text      The text to be displayed
 ************************************************************************/
void drawText(const Point & topLeft, const char * text)
{
   void *pFont = GLUT_BITMAP_HELVETICA_12;  // also try _18

   // prepare to draw the text from the top-left corner
   glRasterPos2f(topLeft.getX(), topLeft.getY());

   // loop through the text
   for (const char *p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
}

/************************************************************************
 * DRAW POLYGON
 * Draw a POLYGON from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the polygon
 *          radius   Size of the polygon
 *          points   How many points will we draw it.  Larger the number,
 *                   the more line segments we will use
 *          rotation True circles are rotation independent.  However, if you
 *                   are drawing a 3-sided polygon (triangle), this matters!
 *************************************************************************/
void drawPolygon(const Point & center, int radius, int points, int rotation)
{
   // begin drawing
   glBegin(GL_LINE_LOOP);

   //loop around a circle the given number of times drawing a line from
   //one point to the next
   for (double i = 0; i < 2 * M_PI; i += (2 * M_PI) / points)
   {
      Point temp(false /*check*/);
      temp.setX(center.getX() + (radius * cos(i)));
      temp.setY(center.getY() + (radius * sin(i)));
      rotate(temp, center, rotation);
      glVertex2f(temp.getX(), temp.getY());
   }

   // complete drawing
   glEnd();

}


/************************************************************************
 * ROTATE
 * Rotate a given point (point) around a given origin (center) by a given
 * number of degrees (angle).
 *    INPUT  point    The point to be moved
 *           center   The center point we will rotate around
 *           rotation Rotation in degrees
 *    OUTPUT point    The new position
 *************************************************************************/
void rotate(Point & point, const Point & origin, int rotation)
{
   // because sine and cosine are expensive, we want to call them only once
   double cosA = cos(deg2rad(rotation));
   double sinA = sin(deg2rad(rotation));

   // remember our original point
   Point tmp(false /*check*/);
   tmp.setX(point.getX() - origin.getX());
   tmp.setY(point.getY() - origin.getY());

   // find the new values
   point.setX(static_cast<int> (tmp.getX() * cosA -
                                tmp.getY() * sinA) +
              origin.getX());
   point.setY(static_cast<int> (tmp.getX() * sinA +
                                tmp.getY() * cosA) +
              origin.getY());
}

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *   INPUT  begin     The position of the beginning of the line
 *          end       The position of the end of the line
 *************************************************************************/
void drawLine(const Point & begin, const Point & end,
              float red, float green, float blue)
{
   // Get ready...
   glBegin(GL_LINES);
   glColor3f(red, green, blue);

   // Draw the actual line
   glVertex2f(begin.getX(), begin.getY());
   glVertex2f(  end.getX(),   end.getY());

   // Complete drawing
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}

/***********************************************************************
 * DRAW Lander
 * Draw a moon-lander spaceship on the screen at a given point
 ***********************************************************************/
void drawLander(const Point & point)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] = 
   {
      {-6, 0}, {-10,0}, {-8, 0}, {-8, 3},  // left foot
      {-5, 4}, {-5, 7}, {-8, 3}, {-5, 4},  // left leg
      {-1, 4}, {-3, 2}, { 3, 2}, { 1, 4}, {-1, 4}, // bottom
      { 5, 4}, { 5, 7}, {-5, 7}, {-3, 7},  // engine square
      {-6,10}, {-6,13}, {-3,16}, { 3,16},   // left of habitat
      { 6,13}, { 6,10}, { 3, 7}, { 5, 7},   // right of habitat
      { 5, 4}, { 8, 3}, { 5, 7}, { 5, 4},  // right leg
      { 8, 3}, { 8, 0}, {10, 0}, { 6, 0}   // right foot
   };

   // draw it
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points) / sizeof(points[0]); i++)
        glVertex2f(point.getX() + points[i].x,
                   point.getY() + points[i].y);

   // complete drawing
   glEnd();
   
   
}


/***********************************************************************
 * DRAW Lander Flame
 * Draw the flames coming out of a moonlander for thrust
 ***********************************************************************/
void drawLanderFlames(const Point & point, 
                      bool bottom,
                      bool left,
                      bool right)
{
   // simple point
   struct PT
   {
      int x;
      int y;
   };

   int iFlame = random(0, 3);  // so the flame flickers
   
   // draw it
   glBegin(GL_LINE_LOOP);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
   
   // bottom thrust
   if (bottom)
   {
      PT points[3][3] =
      {
         { {-5,  -6}, { 0, -1}, { 3, -10} },
         { {-3,  -6}, {-1, -2}, { 0, -15} },
         { { 2, -12}, { 1,  0}, { 6,  -4} }
      };
      
      glVertex2f(point.getX() - 2, point.getY() + 2);
      for (int i = 0; i < 3; i++)
         glVertex2f(point.getX() + points[iFlame][i].x,
                   point.getY() + points[iFlame][i].y);
      glVertex2f(point.getX() + 2, point.getY() + 2);
   }

   // right thrust
   if (right)
   {
      PT points[3][3] =
      {
         { {10, 14}, { 8, 12}, {12, 12} },
         { {12, 10}, { 8, 10}, {10,  8} },
         { {14, 11}, {14, 11}, {14, 11} }
      };
      
      glVertex2f(point.getX() + 6, point.getY() + 12);
      for (int i = 0; i < 3; i++)
         glVertex2f(point.getX() + points[iFlame][i].x,
                    point.getY() + points[iFlame][i].y);
      glVertex2f(point.getX() + 6, point.getY() + 10);
   }

   // left thrust
   if (left)
   {
      PT points[3][3] =
      {
         { {-10, 14}, { -8, 12}, {-12, 12} },
         { {-12, 10}, { -8, 10}, {-10,  8} },
         { {-14, 11}, {-14, 11}, {-14, 11} }
      };
      
      glVertex2f(point.getX() - 6, point.getY() + 12);
      for (int i = 0; i < 3; i++)
         glVertex2f(point.getX() + points[iFlame][i].x,
                    point.getY() + points[iFlame][i].y);
      glVertex2f(point.getX() - 6, point.getY() + 10);
   }

   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
   glEnd();
}


/******************************************************************
 * RANDOM
 * This function generates a random number.  
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}

/******************************************************************
 * RANDOM
 * This function generates a random number.  
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the double
 ****************************************************************/
double random(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   
   assert(min <= num && num <= max);

   return num;
}


/************************************************************************
 * DRAW RECTANGLE
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawRect(const Point & center, int width, int height, int rotation)
{
   Point tl(false /*check*/); // top left
   Point tr(false /*check*/); // top right 
   Point bl(false /*check*/); // bottom left
   Point br(false /*check*/); // bottom right

   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));

   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));

   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));

   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));

   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   glEnd();
}

/************************************************************************
 * DRAW CIRCLE
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *************************************************************************/
void drawCircle(const Point & center, int radius)
{
   assert(radius > 1.0);
   const double increment = 1.0 / (double)radius;

   // begin drawing
   glBegin(GL_LINE_LOOP);

   // go around the circle
   for (double radians = 0; radians < M_PI * 2.0; radians += increment)
      glVertex2f(center.getX() + (radius * cos(radians)),
                 center.getY() + (radius * sin(radians)));
   
   // complete drawing
   glEnd();   
}

/************************************************************************
 * DRAW DOT
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawDot(const Point & point)
{
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 0.8 /* blue % */);
   glBegin(GL_POINTS);

   // Go...
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);

   // Done!  OK, that was a bit too dramatic
   glEnd();
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);
}

void drawRedDot(const Point & point)
{
	glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
	// Get ready, get set...
	glBegin(GL_POINTS);

	// Go...
	glVertex2f(point.getX(), point.getY());
	glVertex2f(point.getX() + 1, point.getY());
	glVertex2f(point.getX() + 1, point.getY() + 1);
	glVertex2f(point.getX(), point.getY() + 1);

	// Done!  OK, that was a bit too dramatic
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
}
/************************************************************************
 * DRAW Tough Bird
 * Draw a tough bird on the screen
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *        hits    How many its remaining to kill the bird 
 *************************************************************************/
void drawToughBird(const Point & center, float radius, int hits)
{
   assert(radius > 1.0);
   const double increment = M_PI / 6.0;
   
   // begin drawing
   glBegin(GL_TRIANGLES);   

   // three points: center, pt1, pt2
   Point pt1(false /*check*/);
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));   
   Point pt2(pt1);

   // go around the circle
   for (double radians = increment;
        radians <= M_PI * 2.0 + .5;
        radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));

      glVertex2f(center.getX(), center.getY());
      glVertex2f(pt1.getX(),    pt1.getY()   );
      glVertex2f(pt2.getX(),    pt2.getY()   );
      
      pt1 = pt2;
   }
      
   // complete drawing
   glEnd();   

   // draw the score in the center
   if (hits > 0 && hits < 10)
   {
      glColor3f(0.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
      glRasterPos2f(center.getX() - 4, center.getY() - 3);
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (char)(hits + '0'));
      glColor3f(1.0, 1.0, 1.0); // reset to white
   }
}

/************************************************************************
 * DRAW Sacred Bird
 * Draw a sacred bird on the screen
 *  INPUT point   The position of the sacred
 *        radius  The size of the bird
 *************************************************************************/
void drawSacredBird(const Point & center, float radius, float rotation)
{
   // begin drawing
   glBegin(GL_LINE_LOOP);
   glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);

   
   //loop around a circle the given number of times drawing a line from
   //one point to the next
   for (int i = 0; i < 5; i++)
   {
      Point temp(false /*check*/);
      float radian = (float)i * (M_PI * 2.0) * 0.4;
      temp.setX(center.getX() + (radius * cos(radian)));
      temp.setY(center.getY() + (radius * sin(radian)));
      rotate(temp, center, rotation);
      glVertex2f(temp.getX(), temp.getY());
   }
   
   // complete drawing
   glColor3f(1.0, 1.0, 1.0); // reset to white
   glEnd();   
}

//same as sacred bird just blue
void drawBlueStar(const Point & center, float radius, float rotation)
{
	// begin drawing
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0 /* red % */, 0.0 /* green % */, 1.0 /* blue % */);


	//loop around a circle the given number of times drawing a line from
	//one point to the next
	for (int i = 0; i < 5; i++)
	{
		Point temp(false /*check*/);
		float radian = (float)i * (M_PI * 2.0) * 0.4;
		temp.setX(center.getX() + (radius * cos(radian)));
		temp.setY(center.getY() + (radius * sin(radian)));
		rotate(temp, center, rotation);
		glVertex2f(temp.getX(), temp.getY());
	}

	// complete drawing
	glColor3f(1.0, 1.0, 1.0); // reset to white
	glEnd();
}

//same as sacred bird just white
void drawWhiteStar(const Point & center, float radius, float rotation)
{
	// begin drawing
	glBegin(GL_LINE_LOOP);

	//loop around a circle the given number of times drawing a line from
	//one point to the next
	for (int i = 0; i < 5; i++)
	{
		Point temp(false /*check*/);
		float radian = (float)i * (M_PI * 2.0) * 0.4;
		temp.setX(center.getX() + (radius * cos(radian)));
		temp.setY(center.getY() + (radius * sin(radian)));
		rotate(temp, center, rotation);
		glVertex2f(temp.getX(), temp.getY());
	}

	// complete drawing
	glEnd();
}

/**********************************************************************
 * DRAW SMALL ASTEROID
 **********************************************************************/
void drawSmallAsteroid( const Point & center, int rotation)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] = 
   {
      {-5, 9},  {4, 8},   {8, 4},   
      {8, -5},  {-2, -8}, {-2, -3}, 
      {-8, -4}, {-8, 4},  {-5, 10}
   };
   glColor3f(0.8 /* red % */, 0.2 /* green % */, 1.0 /* blue % */);
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x, 
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
}

/**********************************************************************
 * DRAW MEDIUM ASTEROID
 **********************************************************************/
void drawMediumAsteroid( const Point & center, int rotation)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] = 
   {
      {2, 8},    {8, 15},    {12, 8}, 
      {6, 2},    {12, -6},   {2, -15},
      {-6, -15}, {-14, -10}, {-15, 0},
      {-4, 15},  {2, 8}
   };
   glColor3f(0.8 /* red % */, 0.2 /* green % */, 1.0 /* blue % */);
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x, 
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
}

/**********************************************************************
 * DRAW LARGE ASTEROID
 **********************************************************************/
void drawLargeAsteroid( const Point & center, int rotation)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   } points[] = 
   {
      {0, 12},    {8, 20}, {16, 14},
      {10, 12},   {20, 0}, {0, -20},
      {-18, -10}, {-20, -2}, {-20, 14},
      {-10, 20},  {0, 12}
   };
   glColor3f(0.8 /* red % */, 0.2 /* green % */, 1.0 /* blue % */);
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(points)/sizeof(PT); i++)
   {
      Point pt(center.getX() + points[i].x, 
               center.getY() + points[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
}


/************************************************************************       
 * DRAW Ship                                                                    
 * Draw a spaceship on the screen                                               
 *  INPUT point   The position of the ship                                      
 *        angle   Which direction it is ponted                                  
 *************************************************************************/
void drawShip(const Point & center, int rotation, bool thrust)
{
   // ultra simple point
   struct PT
   {
      int x;
      int y;
   };
   glColor3f(0.0 /* red % */, 0.9 /* green % */, 1.0 /* blue % */);
   // draw the ship                                                 
   const PT pointsShip[] = 
   { // top   r.wing   r.engine l.engine  l.wing    top
      {0, 6}, {6, -6}, {2, -3}, {-2, -3}, {-6, -6}, {0, 6}  
   };
   
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < sizeof(pointsShip)/sizeof(PT); i++)
   {
      Point pt(center.getX() + pointsShip[i].x, 
               center.getY() + pointsShip[i].y);
      rotate(pt, center, rotation);
      glVertex2f(pt.getX(), pt.getY());
   }
   glEnd();
   glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */);

   // draw the flame if necessary
   if (thrust)
   {
      const PT pointsFlame[3][5] =
      {
         { {-2, -3}, {-2, -13}, { 0, -6}, { 2, -13}, {2, -3} },
         { {-2, -3}, {-4,  -9}, {-1, -7}, { 1, -14}, {2, -3} },
         { {-2, -3}, {-1, -14}, { 1, -7}, { 4,  -9}, {2, -3} }
      };
      
      glBegin(GL_LINE_STRIP);
      glColor3f(1.0 /* red % */, 0.0 /* green % */, 0.0 /* blue % */);
      int iFlame = random(0, 3);
      for (int i = 0; i < 5; i++)
      {
         Point pt(center.getX() + pointsFlame[iFlame][i].x, 
                  center.getY() + pointsFlame[iFlame][i].y);
         rotate(pt, center, rotation);
         glVertex2f(pt.getX(), pt.getY());
      }
      glColor3f(1.0, 1.0, 1.0); // reset to white                                  
      glEnd();
   }
}

/*******************************************************************************************
* DRAW MENU
* Display the menu to screen - before game starts and when requested
*********************************************************************************************/
void drawMenu(const Point & topLeft)
{
	glColor3f(0.2 /* red % */, 0.2 /* green % */, 1.0 /* blue % */); //Should be Blue
	void *pFont = GLUT_BITMAP_TIMES_ROMAN_24;
	// prepare to draw the text from the top-left corner
	glRasterPos2f(topLeft.getX() - 30, topLeft.getY());
	char title[] = { "The Last Stand: Asteroids V" };
	// loop through the text
	for (const char *p = title; *p; p++)
		glutBitmapCharacter(pFont, *p);

	pFont = GLUT_BITMAP_HELVETICA_18;
	glColor3f(1.0 /* red % */, 0.6 /* green % */, 0.0 /* blue % */); //Should be orange
	// prepare to draw the text below title
	glRasterPos2f(topLeft.getX() - 100, topLeft.getY() - 40);
	char modes[] = { "P = Practice   S = Start New Game   H = How to play" };
	// draw text
	for (const char *p = modes; *p; p++)
		glutBitmapCharacter(pFont, *p);

	glRasterPos2f(topLeft.getX() - 75, topLeft.getY() - 62);
	char additionalOptions[] = { "M = Main Menu   Q = Quit   X = Display FPS" };
	for (const char *p = additionalOptions; *p; p++)
		glutBitmapCharacter(pFont, *p);

	glRasterPos2f(topLeft.getX() - 30, topLeft.getY() - 100);
	char passwordOptions[] = { "Z = Enter Password   C = Continue" };
	for (const char *p = passwordOptions; *p; p++)
		glutBitmapCharacter(pFont, *p);

	glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */); //Back to white
}

/**********************************************************************************
* DRAW HOW TO PLAY
* Displays the instructions of how to play
***********************************************************************************/
void drawHowTo(const Point & topLeft)
{
	glColor3f(1.0 /* red % */, 0.6 /* green % */, 0.0 /* blue % */); //Should be orange
	void *pFont = GLUT_BITMAP_TIMES_ROMAN_24;

	/* I know Magic number! Wanted to do this with a string but couldn't figure out how to get it
	to work*/
	char howToPlay[17][55] =
	{
		"Use the space key to fire shots,",
		"Use your Left and Right arrow key to rotate ship.",
		"The W A S D keys also work for arrow keys",
		"The Q and E key will increase speed while turning",
		"The F key will release a shock wave(must be found)",
		"The X key will display your frame rate",
		"The P key will pause while in game.",
		"Avoid hitting all the rocks!",
		"Use the Up arrow key to increase velocity in the",
		"direction the ship is facing",
		"If you succesfully shoot a large rock,",
		"It will split into 2 medium rocks and 1 small rock.",
		"A medium rock will turn into 2 small rocks.",
		"The small rock will disappear upon hitting it.",
		"Red Star is Power Up, It gives one extra life.",
		"Look for what the other colors do.",
		"Push 'H' to return to menu."
	};
	int yMove = 24;
	// loop through the text
	for (int loop = 0; loop < 17; loop++)
	{
		// prepare to draw the text from the top-left corner
		glRasterPos2f(topLeft.getX(), topLeft.getY() - (loop * yMove));
		for (int i = 0; howToPlay[loop][i]; i++)
			glutBitmapCharacter(pFont, howToPlay[loop][i]);
	}
	glColor3f(1.0 /* red % */, 1.0 /* green % */, 1.0 /* blue % */); //Back to white
}

/********************************************************************************
* DRAW HIGH SCORE
* Display the current highscore on screen
********************************************************************************/
void drawHighScore(const Point & topLeft, const int & highScore)
{
	glColor3f(1.0, 0.0, 0.0); //RED
	glRasterPos2f(topLeft.getX(), topLeft.getY());

	void *pFont = GLUT_BITMAP_HELVETICA_18;
	char highScoreText[] = { "High Score:" };
	for (int i = 0; highScoreText[i]; i++)
		glutBitmapCharacter(pFont, highScoreText[i]);
	drawNumber(Point(topLeft.getX() + 20, topLeft.getY() - 10), highScore);
	glColor3f(1.0, 1.0, 1.0); //White
}

/******************************************************************************
* DRAW GAME OVER
* Display after out of AMMO or too many birds have escaped!
******************************************************************************/
void drawGameOver(const Point & topLeft, const int & highScore, const bool & scoreNew)
{
	glColor3f(1.0, 0.0, 0.0); //RED

	void *pFont = GLUT_BITMAP_TIMES_ROMAN_24;

	/* I know Magic number! Wanted to do this with a string but couldn't figure out how to get it
	to work*/
	char gameOver[2][21] =
	{ "Game Over!", "M = Return Main Menu" };
	for (int loop = 0; loop < 2; loop++)
	{
		glRasterPos2f(topLeft.getX() - (loop * 55), topLeft.getY() - (loop * 64));
		for (int i = 0; gameOver[loop][i]; i++)
		{
			glutBitmapCharacter(pFont, gameOver[loop][i]);
			glColor3f(1.0, 0.6, 0.0);  //Yellow
		}
	}

	if (scoreNew)
	{
		glColor3f(1.0, 0.0, 0.0); //RED
		glRasterPos2f(topLeft.getX() - 20, topLeft.getY() - 110);
		char score[] = { "New High Score:" };
		for (int i = 0; score[i]; i++)
		{
			glutBitmapCharacter(pFont, score[i]);
		}
		drawNumber(Point(topLeft.getX() + 45, topLeft.getY() - 125), highScore);
	}
	glColor3f(1.0, 1.0, 1.0);
}

/*************************************************************************************
* DRAW SCORE
* Display current score
*************************************************************************************/
void drawScore(const Point & topLeft, const int & score)
{
	glRasterPos2f(topLeft.getX(), topLeft.getY());

	void *pFont = GLUT_BITMAP_HELVETICA_18;
	char scoreText[] = { "Score:" };
	for (int i = 0; scoreText[i]; i++)
		glutBitmapCharacter(pFont, scoreText[i]);
	drawNumber(Point(topLeft.getX() + 10, topLeft.getY() - 10), score);
}

//displays lives
void drawLives(const Point & bottomLeft, const int & lives)
{
	glRasterPos2f(bottomLeft.getX(), bottomLeft.getY());

	void *pFont = GLUT_BITMAP_HELVETICA_18;
	char livesText[] = { "Lives:" };
	for (int i = 0; livesText[i]; i++)
		glutBitmapCharacter(pFont, livesText[i]);
	drawNumber(Point(bottomLeft.getX() + 10, bottomLeft.getY() - 10), lives);
}


//displays levels
void drawLevel(const Point & bottomRight, const int & level)
{
	glRasterPos2f(bottomRight.getX(), bottomRight.getY());

	void *pFont = GLUT_BITMAP_HELVETICA_18;
	char levelText[] = { "Level:" };
	for (int i = 0; levelText[i]; i++)
		glutBitmapCharacter(pFont, levelText[i]);
	drawNumber(Point(bottomRight.getX() + 10, bottomRight.getY() - 10), level);
}

//displays paused for text in center
void drawPause(const Point & center)
{
	glColor3f(1.0 /* red % */, 0.6 /* green % */, 0.0 /* blue % */); //Should be orange
	glRasterPos2f(center.getX(), center.getY());

	void *pFont = GLUT_BITMAP_HELVETICA_18;
	char pauseText[] = { "PAUSED!" };
	for (int i = 0; pauseText[i]; i++)
		glutBitmapCharacter(pFont, pauseText[i]);
}

//displays currently typed password
void drawPassword(const Point & center, const std::string & password)
{
	static int frame = 0;
	glColor3f(1.0 /* red % */, 0.6 /* green % */, 0.0 /* blue % */); //Should be orange
	glRasterPos2f(center.getX() - 35, center.getY() + 20);

	void *pFont = GLUT_BITMAP_HELVETICA_18;
	for (int i = 0; i < password.size(); i++)
		glutBitmapCharacter(pFont, password[i]);

	if (frame < 7 * FRAMES_VARIABLES_MULTIPLIER)
		glutBitmapCharacter(pFont, '_');
	else if (frame >= 14 * FRAMES_VARIABLES_MULTIPLIER)
		frame = 0;
	frame++;

	glRasterPos2f(center.getX() - 150, center.getY() - 60);
	char complete[] = { "Press Space or Enter when finished" };
	for (int i = 0; complete[i]; i++)
	{
		glutBitmapCharacter(pFont, complete[i]);
	}
	glColor3f(1.0, 1.0, 1.0);
}

//displays if valid password
void drawPasswordResult(const Point & center, const bool & legal)
{
	glColor3f(1.0 /* red % */, 0.6 /* green % */, 0.0 /* blue % */); //Should be orange
	void *pFont = GLUT_BITMAP_HELVETICA_18;
	char anyKey[] = { "Any other key to continue" };
	if (legal)
	{
		char success[][35] = { "Success!", "M = Return Main Menu   R = Retry" };
		for (int loop = 0; loop < 2; loop++)
		{
			glRasterPos2f(center.getX() - 35 - (loop * 100), center.getY() + 64 - (loop * 64));

			for (int i = 0; success[loop][i]; i++)
			{
				glutBitmapCharacter(pFont, success[loop][i]);
			}
		}
		glRasterPos2f(center.getX() - 97, center.getY() - 64);
		for (int i = 0; anyKey[i]; i++)
		{
			glutBitmapCharacter(pFont, anyKey[i]);
		}
	}
	else
	{
		char failure[][35] = { "Invalid Password!", "M = Return Main Menu   R = Retry" };
		for (int loop = 0; loop < 2; loop++)
		{
			glRasterPos2f(center.getX() - 65 - (loop * 70), center.getY() + 64 - (loop * 64));

			for (int i = 0; failure[loop][i]; i++)
			{
				glutBitmapCharacter(pFont, failure[loop][i]);
			}
		}
		glRasterPos2f(center.getX() - 97, center.getY() - 64);
		for (int i = 0; anyKey[i]; i++)
		{
			glutBitmapCharacter(pFont, anyKey[i]);
		}
	}
	glColor3f(1.0, 1.0, 1.0);
}

//displays password for current level
void displayPassword(const Point & center, const std::string & password)
{
	glColor3f(1.0 /* red % */, 0.6 /* green % */, 0.0 /* blue % */); //Should be orange
	glRasterPos2f(center.getX() - 170, center.getY() + 50);
	void *pFont = GLUT_BITMAP_HELVETICA_18;
	char temp[] = { "Password to continue from current spot!" };
	char temp2[] = { "Press C to continue" };
	for (int i = 0; temp[i]; i++)
	{
		glutBitmapCharacter(pFont, temp[i]);
	}
	glRasterPos2f(center.getX() - 30, center.getY());
	for (int i = 0; i < password.size(); i++)
		glutBitmapCharacter(pFont, password[i]);
	glRasterPos2f(center.getX() - 80, center.getY() - 50);
	for (int i = 0; temp2[i]; i++)
	{
		glutBitmapCharacter(pFont, temp2[i]);
	}
	glColor3f(1.0, 1.0, 1.0);
}

//displays if weapon is available
void drawWeaponAvailable(const Point & bottomCenter)
{
	glColor3f(1.0 /* red % */, 0.6 /* green % */, 0.0 /* blue % */); //Should be orange
	glRasterPos2f(bottomCenter.getX(), bottomCenter.getY());

	void *pFont = GLUT_BITMAP_HELVETICA_12;
	char weaponText[] = { "Special Weapon Available! Use F to fire" };
	for (int i = 0; weaponText[i]; i++)
		glutBitmapCharacter(pFont, weaponText[i]);
}
