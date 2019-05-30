/***********************************************************************
 * Header File:
 *    Bullet : houses the information concerning each bullet on screen
 * Author:
 *    Timothy Johnson
 * Summary:
 *    This class is dependent on flyingObject.h and is a derived function of
 *    it. This code will control the direction and speed the bullet travels.
 *    You will need to have a seperate draw function.
 ************************************************************************/
#ifndef BULLET_H
#define BULLET_H

#include "flyingObject.h"
#include "uiDraw.h"

constexpr int BULLET_LIFE = 50; //Increased for screen size
constexpr float BULLET_SPEED = 7.0;


class Bullet : public FlyingObject
{
protected:
	//Bullets don't fly forever.
	int currentLife;

public:
	Bullet() : currentLife(0)
	{
		alive = true;
	}
	//needed to be virtual for background
	virtual ~Bullet() override {  }
	Bullet(const Point & point_, const float & angle, const Velocity & shipV);
	virtual void draw() const override
	{
		drawRedDot(point);
	}

	//Will move the bullet along
	virtual void advance() override;
};


class Background : public Bullet
{
public:
	//Velocity is set up differently with background.
	Background(const Point & point_, const Velocity & backgroundV);
	virtual ~Background() override {   }
	//Draws a different color then bullets
	virtual void draw() const override
	{
		drawDot(point);
	}
};

#endif // !BULLET_H