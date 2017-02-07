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

	// Newton's stuff
	cbmath::vec3 acceleration;
	cbmath::vec3 force;
	float mass, massInv;
};


// force
void addForce(egpfwMover *mover, const cbmath::vec3 force)
{
	mover->force += force;
}


// mass
void setMass(egpfwMover *mover, const float mass)
{
	if (mass > 0.0f)
		mover->massInv = 1.0f / (mover->mass = mass);
	else
		mover->massInv = (mover->mass = 0.0f);
}


// acceleration
void updateAcceleration(egpfwMover *mover)
{
	// F = ma ---> a = F/m
	mover->acceleration = (mover->force * mover->massInv) + mover->accelerationFixed;
	mover->force.set();
}


// physics (integration) updates
void updateMoverFirstOrder(egpfwMover *mover, const float dt)
{
	mover->position += mover->velocity * dt;
	mover->velocity += mover->acceleration * dt;

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