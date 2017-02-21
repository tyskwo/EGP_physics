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
    
    // a reference to the spring, if we are one
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

	// drag
	float dragCoefficient, surfaceArea;
    
    
private:
    
	void init();

    // internal methods to update our velocity and acceleration
	void updateVelocity(const float dt);
	void updateAcceleration();
	
    

public:
    
    // constructor and destructor
	Mover();
	Mover(float dragCoeff, float surfArea);
	~Mover();

    // sets the anchor to this object if it is a spring
    // written by: Wednesday-David
	void setSpringAnchor(cbmath::vec3 anchor);

    // set the mass of this object
    // written by: Ty
	void setMass(float mass);

    // adds a force to this object
    // written by: Ty
	void addForce(const cbmath::vec3 force);

	cbmath::vec3 calculateDrag(const float fluidDensity);

    // uses displacement to determine the mover's new position given the elapsed time
    // written by: Ty
	void updateMoverDisplacement(const float dt);

    // called to update the mover's graphical representation to match the data
    // written by: Ty
	void updateMoverGraphics();

    // called to create a simple collision response to the ground plane
    // written by: Ty and Wednesday-David
	void clampMoverToGround(const float groundHeight, const float restitution);
};





#endif	// __MOVER_H
