/***********************************************************************
 * Header File:
 *    Rocks : houses the information concerning each rock on screen
 * Author:
 *    Timothy Johnson
 * Summary:
 *    This class is dependent on flyingObject.h and is a derived function of
 *    it. This code will control the direction and speed the rocks travel.
 *    You will need to have a seperate draw function.
 ************************************************************************/
#ifndef rocks_h
#define rocks_h

#include "flyingObject.h"
#include "uiDraw.h"
#include <vector>

//These variables will hold each rocks radius
constexpr int BIG_ROCK_SIZE = 16;
constexpr int MEDIUM_ROCK_SIZE = 12;
constexpr int SMALL_ROCK_SIZE = 6;

//These Variables will control how fast each rock spins
constexpr float BIG_ROCK_SPIN = 2.0;
constexpr float MEDIUM_ROCK_SPIN = 5.0;
constexpr float SMALL_ROCK_SPIN = 9.0;

//These Variables will hold the rocks value
constexpr int BIG_ROCK_VALUE = 1;
constexpr int MEDIUM_ROCK_VALUE = 1;
constexpr int SMALL_ROCK_VALUE = 2;

class Rocks : public FlyingObject
{
protected:
	float rotation;
public:
	Rocks() { }
	//Virtual destructor so child classes can destructor properly
	virtual ~Rocks() override { };
	float getRotation() const { return rotation; }
	//Purely virtual since each rock looks different.
	virtual void draw() const = 0;
	//Purely Virtual since each rock moves differently.
	virtual void advance() = 0;
	//Purely Virtual since each rock is worth a different value.
	virtual int getValue() const = 0;
	//Purely Virtual since each rock has a different radius.
	virtual int getRadius() const = 0;
	//Purely Virtual since each rock will have a different death outcome.
	virtual std::vector<Rocks*> killRocks() = 0;
	
	//Purely Virtual since each rock will return a different value.
	//this will only be used when a rock is completely destroyed
	virtual int hit() = 0;
};

class BigRock : public Rocks
{
protected:
	//These variables only need to be stored once for all instances of BigRock
	const static int radius = BIG_ROCK_SIZE;
	const static int value = BIG_ROCK_VALUE;
public:
	//For making a new BigRock
	BigRock(const Point & point_);
	virtual ~BigRock() override {   }
	virtual void draw() const override
	{
		drawLargeAsteroid(point, rotation);
	}
	virtual void advance() override;
	virtual int getValue() const override { return value; }
	int getRadius() const override { return radius; }
	virtual std::vector<Rocks*> killRocks() override;

	virtual int hit() override { return (5 * SMALL_ROCK_VALUE) + (2 * MEDIUM_ROCK_VALUE) + BIG_ROCK_VALUE; }
};

class MediumRock : public Rocks
{
protected:
	//These variables only need to be stored once for all instances of MediumRock
	const static int radius = MEDIUM_ROCK_SIZE;
	const static int value = MEDIUM_ROCK_VALUE;
public:
	virtual ~MediumRock() override {   }
	//For making a new MediumRock
	MediumRock(const Point & point_);
	//For when BigRock dies
	MediumRock(const Point & point_, const Velocity & speed_, const bool & up);
	virtual void draw() const override
	{
		drawMediumAsteroid(point, rotation);
	}
	virtual int getValue() const override { return value; }
	virtual void advance() override;
	int getRadius() const override { return radius; }
	virtual std::vector<Rocks*> killRocks() override;

	virtual int hit() override { return (2 * SMALL_ROCK_VALUE) + MEDIUM_ROCK_VALUE; }
};

class SmallRock : public Rocks
{
protected:
	//These variables only need to be stored once for all instances of SmallRock
	const static int radius = SMALL_ROCK_SIZE;
	const static int value = SMALL_ROCK_VALUE;
public:
	virtual ~SmallRock() override {   }
	//For making a new SmallRock
	SmallRock(const Point & point_);
	//For when BigRock dies
	SmallRock(const Point & point_, const Velocity & speed_);
	//For when MediumRock dies
	SmallRock(const Point & point_, const Velocity & speed_, const bool & left);
	virtual int getValue() const override { return value; }
	virtual void draw() const override
	{
		drawSmallAsteroid(point, rotation);
	}
	virtual void advance() override;

	int getRadius() const override { return radius; }
	virtual std::vector<Rocks*> killRocks() override; 
	virtual int hit() override { return SMALL_ROCK_VALUE; }
};

#endif /* rocks_h */
