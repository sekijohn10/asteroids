/*********************************************************************
 * File: bullet.cpp
 * Description: Contains the implementaiton of the bullet class
 *  methods.
 *********************************************************************/
#include "bullet.h"
#include "uiInteract.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES 
#include "math.h"

#else // __linux__
#include <cmath>

#endif  //_WIN32

//Will create background differently then bullet
Background::Background(const Point & point_, const Velocity & backgroundV)
{
	speed.setDx(backgroundV.getDx());
	speed.setDy(backgroundV.getDy());
	point = point_;
	currentLife = 0; 
	alive = true;
}

//Will create bullet according to ship speed
Bullet::Bullet(const Point & point_, const float & angle, const Velocity & shipV)
{
	float dx;
	float dy;

	dx = ((BULLET_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle))) + shipV.getDx();
	dy = ((BULLET_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle))) + shipV.getDy();

	speed.setDx(dx);
	speed.setDy(dy);
	point = point_;
	currentLife = 0;
	alive = true;
}

//Move along the bullets
void Bullet::advance()
{
	if (currentLife == (BULLET_LIFE * FRAMES_VARIABLES_MULTIPLIER))
		kill();
	else
	{
		point.addX(speed.getDx());
		point.addY(speed.getDy());
		currentLife++;
	}
}