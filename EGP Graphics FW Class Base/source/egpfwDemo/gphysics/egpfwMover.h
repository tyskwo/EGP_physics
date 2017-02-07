// Mover structure for integration
// By D. Buckstein
// Modified by: 

#ifndef __EGPFW_MOVER_H
#define __EGPFW_MOVER_H


#include "cbmath/cbtkMatrix.h"


struct egpfwMover
{
	// graphics: leave it at the start of the structure for consistency
	cbmath::mat4 modelMatrix;

    
    
    
	// movement
	cbmath::vec3 position, velocity, accelerationFixed;
    
    
    // Newton equations
    cbmath::vec3 force;
    cbmath::vec3 acceleration;
    
    // mass
    float mass, massInverse;
};





void setMass(egpfwMover *mover, float mass)
{
    if(mass > 0.0f) mover->massInverse = 1 / (mover->mass = mass);
    else            mover->mass = mover->massInverse = 0.0f;
}



void addForce(egpfwMover *mover, const cbmath::vec3 force)
{
    mover->force += force;
}



// acceleration
void updateAcceleration(egpfwMover *mover)
{
	// F = ma ---> a = F/m
	mover->acceleration = (mover->force * mover->massInverse) + mover->accelerationFixed;
	mover->force.set();
}


// physics (integration) updates




void updateVelocity(egpfwMover *mover, const float dt)
{
    // v = v0 + a*dt
    mover->velocity += mover->acceleration * dt;
}



void updateMoverFirstOrder(egpfwMover *mover, const float dt)
{
	mover->position += mover->velocity * dt;
	mover->velocity += mover->acceleration * dt;

	updateAcceleration(mover);
}



void updateMoverDisplacement(egpfwMover *mover, const float dt)
{
    //x = x0 + v * dt + 0.5 * a * dt^2
    mover->position += mover->velocity * dt + 0.5 * mover->acceleration * dt * dt;
    
    updateVelocity(mover, dt);
    updateAcceleration(mover);
}





// graphics updates
void updateMoverGraphics(egpfwMover *mover)
{
	mover->modelMatrix.c3.xyz = mover->position;
}


// ****
// clamp to ground (simple bounce): 
//	if object goes into ground, clamp it to the ground and 
//	use the remaining velocity for a bounce
void clampMoverToGround(egpfwMover *mover, const float groundHeight, const float restitution)
{

}



#endif	// __EGPFW_MOVER_H
