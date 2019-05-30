/*********************************************************************
 * File: ship.cpp
 * Description: Contains the implementaiton of the ship class
 *  methods.
 *********************************************************************/
#include "ship.h"

//only included for frames per second multiplier
#include "uiInteract.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES 
#include "math.h"

#else // __linux__
#include <cmath>

#endif  //_WIN32

Ship::Ship()
{
	alive = true;
	point.setX(0);
	point.setY(0);
	angle = 90;
}

void Ship::draw() const
{	
	Interface ui;
	drawShip(point, angle + 270, (ui.isUp() || ui.isStrafeL() || ui.isStrafeR()));	
}

void Ship::rotate(const bool & left)
{
	if (left)
		angle += (ROTATE_AMOUNT / FRAMES_VARIABLES_MULTIPLIER);
	else
		angle -= (ROTATE_AMOUNT / FRAMES_VARIABLES_MULTIPLIER);
	if (angle <= 0)
		angle += 360;
	else if (angle > 360)
		angle -= 360;
}

void Ship::thrust()
{
	speed.setDx(speed.getDx() + ((THRUST_AMOUNT / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle))));
	speed.setDy(speed.getDy() + ((THRUST_AMOUNT / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle))));
}

void Ship::reverseThruster() // Similar to stabilizers
{
	speed.setDx(speed.getDx() + ((REVERSE_THRUST_AMOUNT / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * (angle + 180)))));
	speed.setDy(speed.getDy() + ((REVERSE_THRUST_AMOUNT / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * (angle + 180)))));
}

void Ship::strafe(const bool & isLeft)
{
	rotate(isLeft);
	speed.setDx(speed.getDx() + ((THRUST_STRAFE / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle))));
	speed.setDy(speed.getDy() + ((THRUST_STRAFE / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle))));
}

void Ship::reset()
{
	alive = true;
	point.setX(0);
	point.setY(0);
	speed.setDx(0);
	speed.setDy(0);
	angle = 90;
}
