/***********************************************************************
 * Header File:
 *    Flying Object : A generic class for any flying objects on screen
 * Author:
 *    Timothy Johnson
 * Summary:
 *    This class is a base class that can be implemented with any object that
 *    consists of a Point and Velocity. This class does require you to have
 *    Velocity and Point classes included in code.
 ************************************************************************/
#ifndef FLYING_OBJECT_H
#define FLYING_OBJECT_H

#include "velocity.h"
#include "point.h"

class FlyingObject
{
protected:
	//Is the object alive?
	bool alive;
	//Where is it located?
	Point point;
	//How fast is it going?
	Velocity speed;
public:
	FlyingObject() : point(Point()), speed(Velocity()), alive(true) {  }
	//virtual destructor for child classes.
	virtual ~FlyingObject() { };
	Point getPoint() const
	{   
		return point;  
	}

	bool isAlive() const
	{
		return alive;
	}

	void setPoint(const Point & point_)
	{
		point = point_;
	}

	void setVelocity(const Velocity & speed_)
	{
		speed = speed_;
	}

	Velocity getVelocity() const
	{
		return speed;
	}

	void kill()
	{
		alive = false;
	}

	//needs to purely be virtual, no way to draw every object that can fly.
	virtual void draw() const = 0;

	//Virtual for those objects that move differently
	virtual void advance();
	void flipSide(const bool & isY, const bool & isX);
};

#endif  // !FLYING_OBJECT_H