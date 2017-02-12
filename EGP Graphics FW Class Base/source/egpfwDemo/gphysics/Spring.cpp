//  Wednesday-David Hartman 0923439
//  Ty Wood					0901793
//
//  EGP 425.01
//  Simple Particle Engine
//  17.02.10
//
//  We certify that this work is entirely our own.
//  The assessor of this project may reproduce this project and provide copies to other academic staff,
//  and/or communicate a copy of this project to a plagiarism-checking service,
//  which may retain a copy of the project on its database.

#include "Spring.h"

#include <math.h>

Spring::Spring(cbmath::vec3 _anchor)
:anchor(_anchor)
{
	lRest = 1.0f;
}

void Spring::updateSpring(const cbmath::vec3 position)
{
	xPrev = x;
	x = anchor - position;
	v = x - xPrev;

	l = cbmath::length(x);
	vl = cbmath::length(v);
	dl = l - lRest;
	xDir = x / l;
	vDir = v / vl;
}

float Spring::getSpringCoefficient(const float mass, const float dt)
{
	// k = m / dt^2
	return (mass / (dt*dt));
}

float Spring::getDampingCoefficient(const float mass, const float dt)
{
	return (mass / dt);
}

cbmath::vec3 Spring::getForceStiffSpringDamped(const float stiff, const float damp)
{
	//	f = -k*C0x - dC1v
	const float stiffSpring = stiff * dl;
	const float dampSpring = damp * vl;
	const cbmath::vec3 f = stiffSpring * xDir + dampSpring * vDir;
	return f;
}