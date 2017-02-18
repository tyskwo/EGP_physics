//  Mover structure for integration
//  By D. Buckstein
//	This file was modified by Wednesday-David Hartman, Ty Wood with permission from author.



#ifndef __MOVER_H
#define __MOVER_H

#define GRAVITATIONAL_CONSTANT -9.81f

#include <math.h>

#include "cbmath/cbtkMatrix.h"

#ifdef _WIN32
    #include "..\gphysics\Spring.h"
#else	// !_WIN32
    #include "Spring.h"
#endif	// _WIN32



class Mover
{
    
public:
	Spring* m_spring;

	// graphics: leave it at the start of the structure for consistency
	cbmath::mat4 modelMatrix;

	// movement
	cbmath::vec3 position, velocity, accelerationFixed;    
    
    // Newton equations
    cbmath::vec3 force;
    cbmath::vec3 acceleration;
    
    // mass
    float mass, massInverse;

    
    
private:
	void updateVelocity(const float dt);
	void updateAcceleration();
	
    

public:
	Mover();
	~Mover();

	void setSpringAnchor(cbmath::vec3 anchor);

	void setMass(float mass);

	void addForce(const cbmath::vec3 force);

	void updateMoverDisplacement(const float dt);

	void updateMoverGraphics();

	void clampMoverToGround(const float groundHeight, const float restitution);
};

#endif	// __MOVER_H