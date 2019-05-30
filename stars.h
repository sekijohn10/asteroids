/***********************************************************************
 * Header File:
 *    Stars : houses the information concerning the Stars
 * Author:
 *    Timothy Johnson
 * Summary:
 *    This class is dependent on flyingObject.h and is a derived function of
 *    it. This code will control the direction and speed the stars travel.
 *    You will need to have a seperate draw function.
 ************************************************************************/
#ifndef STARS_H
#define STARS_H

#include "flyingObject.h"
#include "uiDraw.h"

//These variables will represent a value for game to recognize which star was hit
constexpr int RED_STAR_VALUE = 1;
constexpr int BLUE_STAR_VALUE = 2;
constexpr int WHITE_STAR_VALUE = 3;

class Stars : public FlyingObject
{
protected:
	//What direction is star facing?
	float rotation;
	//This variable will never change
	const static int radius = 10;
	//Stars are temporary
	int currentLife;
public:
	Stars() { }
	//Virtual destructor for child classes
	virtual ~Stars() override { };
	float getRotation() const { return rotation; }
	//Purely Virtual since all stars are different color
	virtual void draw() const = 0;
	void advance() override;
	//Purely Virtual since each star has a different value
	virtual int getValue() const = 0;
	virtual int getRadius() const { return radius; }
};

class RedStar : public Stars
{
protected:
	//This variable will never change
	const static int value = RED_STAR_VALUE;
public:
	virtual ~RedStar() override {  }
	RedStar(const Point & point_);
	virtual void draw() const override
	{
		drawSacredBird(point, radius, rotation);
	}
	virtual int getValue() const override { return value; }
};

class BlueStar : public Stars
{
protected:
	//This variable will never change
	const static int value = BLUE_STAR_VALUE;
public:
	virtual ~BlueStar() override {  }
	BlueStar(const Point & point_);
	virtual void draw() const override
	{
		drawBlueStar(point, radius, rotation);
	}
	virtual int getValue() const override { return value; }
};

class WhiteStar : public Stars
{
protected:
	//This variable will never change
	const static int value = WHITE_STAR_VALUE;
public:
	virtual ~WhiteStar() override {  }
	WhiteStar(const Point & point_);
	virtual void draw() const override
	{
		drawWhiteStar(point, radius, rotation);
	}
	virtual int getValue() const override { return value; }
};

#endif // STARS_H