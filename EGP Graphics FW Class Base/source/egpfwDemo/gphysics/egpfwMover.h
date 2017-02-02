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
	cbmath::vec3 position, velocity, acceleration;
};


// physics (integration) updates
void updateMoverFirstOrder(egpfwMover *mover, const float dt)
{
	mover->position += mover->velocity * dt;
	mover->velocity += mover->acceleration * dt;
}


// graphics updates
void updateMoverGraphics(egpfwMover *mover)
{
	mover->modelMatrix.c3.xyz = mover->position;
}



#endif	// __EGPFW_MOVER_H