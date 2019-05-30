/*********************************************************************
 * File: rocks.cpp
 * Description: Contains the implementaiton of the rocks class
 *  methods.
 *********************************************************************/
#include "rocks.h"
#include "uiInteract.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES 
#include "math.h"

#else // __linux__
#include <cmath>

#endif  //_WIN32

constexpr float BIG_ROCK_SPEED = 1.0;
constexpr float MEDIUM_ROCK_SPEED = 2.0;   //used in levels
constexpr float SMALL_ROCK_SPEED = 3.5;    // "         "
constexpr float LARGE_TO_SMALL_SPEED = 2.0;
constexpr float LARGE_TO_MEDIUM_SPEED = 1.0;
constexpr float MEDIUM_TO_SMALL_SPEED = 3.0;

void BigRock::advance()
{
	point.addX(speed.getDx());
	point.addY(speed.getDy());
	rotation += BIG_ROCK_SPIN / FRAMES_VARIABLES_MULTIPLIER;
}

std::vector<Rocks*> BigRock::killRocks()
{
	alive = false;

	std::vector<Rocks*> temp;
	Rocks* rock1 = NULL;
	Rocks* rock2 = NULL;
	Rocks* rock3 = NULL;
	rock1 = new MediumRock(point, speed, true);
	rock2 = new MediumRock(point, speed, false);
	rock3 = new SmallRock(point, speed);
	temp.push_back(rock1);
	temp.push_back(rock2);
	temp.push_back(rock3);
	
	return temp;
}

BigRock::BigRock(const Point & point_)
{
	int angle = random(1, 361);
	float dx;
	float dy;
	
	dx = (BIG_ROCK_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle));
	dy = (BIG_ROCK_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle));

	speed.setDx(dx);
	speed.setDy(dy);
	point = point_;
	rotation = random(0, 361);
	alive = true;
}

MediumRock::MediumRock(const Point & point_)
{
	int angle = random(1, 361);
	float dx;
	float dy;

	dx = (MEDIUM_ROCK_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle));
	dy = (MEDIUM_ROCK_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle));

	speed.setDx(dx);
	speed.setDy(dy);
	point = point_;
	rotation = random(0, 361);
	alive = true;
}

MediumRock::MediumRock(const Point & point_, const Velocity & speed_, const bool & up)
{
	float dy;
	if (up)
		dy = (LARGE_TO_MEDIUM_SPEED / FRAMES_VARIABLES_MULTIPLIER);
	else
		dy = -(LARGE_TO_MEDIUM_SPEED / FRAMES_VARIABLES_MULTIPLIER);

	speed.setDx(speed_.getDx());
	speed.setDy(speed_.getDy() + dy);
	point = point_;
	rotation = random(0, 361);
	alive = true;
}

void MediumRock::advance()
{
	point.addX(speed.getDx());
	point.addY(speed.getDy());
	rotation += (MEDIUM_ROCK_SPIN / FRAMES_VARIABLES_MULTIPLIER);
}

std::vector<Rocks*> MediumRock::killRocks()
{
	alive = false;

	std::vector<Rocks*> temp;
	Rocks* rock1 = NULL;
	Rocks* rock2 = NULL;
	rock1 = new SmallRock(point, speed, true);
	rock2 = new SmallRock(point, speed, false);
	temp.push_back(rock1);
	temp.push_back(rock2);

	return temp;
}

SmallRock::SmallRock(const Point & point_)
{
	int angle = random(1, 361);
	float dx;
	float dy;

	dx = (SMALL_ROCK_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (cos(M_PI / 180.0 * angle));
	dy = (SMALL_ROCK_SPEED / FRAMES_VARIABLES_MULTIPLIER) * (sin(M_PI / 180.0 * angle));

	speed.setDx(dx);
	speed.setDy(dy);
	point = point_;
	rotation = random(0, 361);
	alive = true;
}

SmallRock::SmallRock(const Point & point_, const Velocity & speed_)
{
	speed.setDx(speed_.getDx() + (LARGE_TO_SMALL_SPEED / FRAMES_VARIABLES_MULTIPLIER));
	speed.setDy(speed_.getDy());
	point = point_;
	rotation = random(0, 361);
	alive = true;
}

SmallRock::SmallRock(const Point & point_, const Velocity & speed_, const bool & left)
{
	float dx;
	if (left)
		dx = -(MEDIUM_TO_SMALL_SPEED / FRAMES_VARIABLES_MULTIPLIER);
	else
		dx = (MEDIUM_TO_SMALL_SPEED / FRAMES_VARIABLES_MULTIPLIER);

	speed.setDx(speed_.getDx() + dx);
	speed.setDy(speed_.getDy());
	point = point_;
	rotation = random(0, 361);
	alive = true;
}

void SmallRock::advance()
{
	point.addX(speed.getDx());
	point.addY(speed.getDy());
	rotation += (SMALL_ROCK_SPIN / FRAMES_VARIABLES_MULTIPLIER);
}

std::vector<Rocks*> SmallRock::killRocks()
{
	alive = false;
	std::vector<Rocks*> temp;
	return temp;
}
