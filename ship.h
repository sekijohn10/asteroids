/***********************************************************************
 * Header File:
 *    Ship : houses the information concerning the ship
 * Author:
 *    Timothy Johnson
 * Summary:
 *    This class is dependent on flyingObject.h and is a derived function of
 *    it. This code will control the direction and speed the ship travels.
 *    You will need to have a seperate draw function.
 ************************************************************************/
#ifndef ship_h
#define ship_h

#include "flyingObject.h"
#include "uiDraw.h"

//These variables will hold the ships radius and thrust power
constexpr int SHIP_SIZE = 6;
constexpr float REVERSE_THRUST_AMOUNT = 0.1;
constexpr float ROTATE_AMOUNT = 6.0;
constexpr float THRUST_AMOUNT = 0.5;
constexpr float THRUST_STRAFE = 0.4;


class Ship : public FlyingObject
{
private:
	//Radius never changes
	const static int radius = SHIP_SIZE;
	//What direction is the ship facing?
	float angle;

public:
	Ship();
	virtual ~Ship() override {  }
	virtual void draw() const override;

	void rotate(const bool & left);
	void thrust();
	void reverseThruster();
	void strafe(const bool & isLeft);
	float getAngle() const { return angle; }
	int getRadius() const { return radius; }

	//Will change all variables for ship back to initial setup
	void reset();
};

#endif /* ship_h */
