//  Mover structure for integration
//  By D. Buckstein
//	This file was modified by David Hartman, Ty Wood with permission from author.

#ifndef __EGPFW_MOVER_H
#define __EGPFW_MOVER_H


#include "cbmath/cbtkMatrix.h"
#include <math.h>


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
    float mass, massInverse, damping;
};





void setMass(egpfwMover *mover, float mass)
{
    if(mass > 0.0f) mover->massInverse = 1.0f / (mover->mass = mass);
    else            mover->massInverse = mover->mass = 0.0f;
}

void setDamping(egpfwMover *mover, float damping)
{
	mover->damping = damping;
}



void addForce(egpfwMover *mover, const cbmath::vec3 force)
{
    mover->force += force;
}




// physics (integration) updates




void updateVelocity(egpfwMover *mover, const float dt)
{
    // v = v0 + a*dt
    mover->velocity += mover->acceleration * dt;

	mover->velocity *= powf(mover->damping, dt);
}



void updateAcceleration(egpfwMover *mover)
{
    // a = F/m = F * im
    mover->acceleration = (mover->force + mover->accelerationFixed) * mover->massInverse;
    
    mover->force.set();
}



void updateMoverDisplacement(egpfwMover *mover, const float dt)
{
    //x = x0 + v * dt + 0.5 * a * dt^2
    mover->position += mover->velocity * dt + 0.5 * mover->acceleration * (dt * dt);
    
    updateVelocity(mover, dt);
    updateAcceleration(mover);
}





// graphics updates
void updateMoverGraphics(egpfwMover *mover)
{
	mover->modelMatrix.c3.xyz = mover->position;
}



#endif	// __EGPFW_MOVER_H
