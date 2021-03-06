// Force generator functions
// By D. Buckstein
// Modified by: 

#ifndef __EGPFW_FORCES_H
#define __EGPFW_FORCES_H


#include "cbmath/cbtkMatrix.h"


// ****
// drag: calculate using traditional equation: 
// f = (c*d*a*v^2)/2
//	where c is the drag coefficient of the object, 
//	fluid density is that of the fluid the object is travelling through, 
//	surface area is the area on the object affected by drag, and 
//	object velocity is the velocity of the object relative to the fluid
cbmath::vec3 getForceDrag(const float dragCoeff, const float surfaceArea, const float fluidDensity, const cbmath::vec3 objectVelocity)
{
    cbmath::vec3 square = cbmath::vec3(objectVelocity.x*objectVelocity.x,
                                       objectVelocity.y*objectVelocity.y,
                                       objectVelocity.z*objectVelocity.z);

	return (dragCoeff*fluidDensity*surfaceArea*(square)) / 2;
}



// spring
struct egpfwSpring
{
	const cbmath::vec3 *anchor, *object;
	float lRest, l, dl, vl;
	cbmath::vec3 x, v, xDir, vDir, xPrev;
};

// ****
void updateSpring(egpfwSpring *spring)
{
	// direction of force will be towards anchor
    spring->xPrev = spring->x;
    spring->x     = *spring->anchor - *spring->object;
    
    spring->v = spring->x - spring->xPrev;
    
    spring->l  = cbmath::length(spring->x);
    spring->vl = cbmath::length(spring->v);
    spring->dl = spring->l - spring->lRest;
    
    spring->xDir = spring->x / spring->l;
    spring->vDir = spring->v / spring->vl;
}


// get the exact stiffness parameter needed to return to resting position
float getSpringCoefficient(const float mass, const float dt)
{
	// k = m / dt^2
	return (mass / (dt*dt));
}


// get the exact damping parameter needed to return to resting position
float getDampingCoefficient(const float mass, const float dt)
{
    return (mass / dt);
}


// ****
// spring: vector implementation of Hooke's law: f = -k*dl
//	where k is spring stiffness coefficient, dl is the change in the spring's 
//	length from its resting state (dl = l - l0)
cbmath::vec3 getForceStiffSpring(const egpfwSpring *spring, const float stiff)
{
	// if f = -k(l - l0), then 
	// vector implementation is F = k(|x| - l0) * x/|x|, 
	//	where x is the vector from the endpoint towards the anchor
	// the formula experiences the STIFF SPRING PROBLEM: unwanted bouncing
    
    const float stiffSpring = stiff * spring->dl;
    const cbmath::vec3 f = stiffSpring * spring->xDir;
    return f;
}

// ****
// spring: stable implementation: 
//	https://www.gamedev.net/resources/_/technical/math-and-physics/towards-a-simpler-stiffer-and-more-stable-spring-r3227
//	f = -k*C0x - dC1v
cbmath::vec3 getForceStiffSpringDamped(const egpfwSpring *spring, const float stiff, const float damp)
{
    const float stiffSpring = stiff * spring->dl;
    const float dampSpring  = damp  * spring->vl;
    const cbmath::vec3 f = stiffSpring * spring->xDir + dampSpring * spring->vDir;
    return f;
}


#endif	// __EGPFW_FORCES_H
