/***********************************************************************
 * Header File
 * Velocity : Will stroe the Velocity of any object
 * Author:
 *    Timothy Johnson
 * Summary:
 *    This class will store the speed of an object. It doesn't depend
 *    On any other files. It is therefore transferable to any project you 
 *    may have. If you cross this with other applications you can 
 *    use this to determine the velocity of an object in a 2-D environment.
 ************************************************************************/
#ifndef VELOCITY_H
#define VELOCITY_H

class Velocity
{
private:
	float dx;
	float dy;
public:
	// DEFAULT CONSTRUCTOR
	Velocity() : dx(0), dy(0) {   }
	
	// NON-DEFAULT CONSTRUCTOR
	Velocity(float dx, float dy) : dx(dx), dy(dy) {   }
	
	// VELOCITY :: GET DX
	float getDx() const
	{
		return dx;
	}

	// VELOCITY :: GET DY
	float getDy() const
	{
		return dy;
	}

	// VELOCITY :: SET DX
	void setDx(float dx)
	{
		this->dx = dx;
	}

	// VELOCITY :: SET DY
	void setDy(float dy)
	{
		this->dy = dy;
	}
};
#endif // !VELOCITY_H