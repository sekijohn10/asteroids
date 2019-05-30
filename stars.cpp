/*********************************************************************
 * File: stars.cpp
 * Description: Contains the implementaiton of the stars class
 *  methods.
 *********************************************************************/
#include "stars.h"
#include "uiInteract.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES 
#include "math.h"

#else // __linux__
#include <cmath>

#endif  //_WIN32

constexpr float STAR_ROTATION = 6.0;
constexpr float STAR_SPEED = 2.0;
constexpr int STAR_LIFE = 600;

RedStar::RedStar(const Point & point_)
{
	int angle = random(1, 361);
	float dx;
	float dy;

	dx = (STAR_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle));
	dy = (STAR_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle));

	speed.setDx(dx);
	speed.setDy(dy);
	point = point_;
	rotation = random(0, 361);
	alive = true;
	currentLife = 0;
}

void Stars::advance()
{
	if (currentLife == (STAR_LIFE * FRAMES_VARIABLES_MULTIPLIER))
		kill();
	else
	{
		point.addX(speed.getDx());
		point.addY(speed.getDy());
		rotation += STAR_ROTATION / FRAMES_VARIABLES_MULTIPLIER;
		currentLife++;
	}
}

BlueStar::BlueStar(const Point & point_)
{
	int angle = random(1, 361);
	float dx;
	float dy;

	dx = (STAR_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle));
	dy = (STAR_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle));

	speed.setDx(dx);
	speed.setDy(dy);
	point = point_;
	rotation = random(0, 361);
	alive = true;
	currentLife = 0;
}

WhiteStar::WhiteStar(const Point & point_)
{
	int angle = random(1, 361);
	float dx;
	float dy;

	dx = (STAR_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle));
	dy = (STAR_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle));

	speed.setDx(dx);
	speed.setDy(dy);
	point = point_;
	rotation = random(0, 361);
	alive = true;
	currentLife = 0;
}
