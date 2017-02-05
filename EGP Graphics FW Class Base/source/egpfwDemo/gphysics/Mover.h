//  Mover structure for integration
//  By D. Buckstein
//	This file was modified by Wednesday-David Hartman, Ty Wood with permission from author.



#ifndef __MOVER_H
#define __MOVER_H

#define GRAVITATIONAL_CONSTANT -9.81f

#include <math.h>

#include "cbmath/cbtkMatrix.h"



class Mover
{
    
public:
	// graphics: leave it at the start of the structure for consistency
	cbmath::mat4 modelMatrix;

	// movement
	cbmath::vec3 position, velocity, accelerationFixed;    
    
    // Newton equations
    cbmath::vec3 force;
    cbmath::vec3 acceleration;
    
    // mass
    float mass, massInverse, damping;

    
    
private:
	void updateVelocity(const float dt);
	void updateAcceleration();
	
    

public:
	Mover();
	~Mover();

	void setMass(float mass);
	void setDamping(float damping);

	void addForce(const cbmath::vec3 force);

	void updateMoverDisplacement(const float dt);

	void updateMoverGraphics();
};

#endif	// __MOVER_H
