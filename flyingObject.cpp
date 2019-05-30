/*********************************************************************
 * File: flyingObject.cpp
 * Description: Contains the implementaiton of the flyingObject class
 *  methods.
 *********************************************************************/
#include "flyingObject.h"

//Move the object along
void FlyingObject::advance()
{
	point.addX(speed.getDx());
	point.addY(speed.getDy());
}

//If off screen have move to the otherside with same velocity and direction
void FlyingObject::flipSide(const bool & isY, const bool & isX)
{
	if (isY)
		point.setY(-point.getY());
	if (isX)
		point.setX(-point.getX());
}