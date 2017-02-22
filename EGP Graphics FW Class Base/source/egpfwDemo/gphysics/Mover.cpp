//  Wednesday-David Hartman 0923439
//  Ty Wood					0901793
//
//  EGP 425.01
//  Simple Particle Engine
//  17.02.05
//
//  We certify that this work is entirely our own.
//  The assessor of this project may reproduce this project and provide copies to other academic staff,
//  and/or communicate a copy of this project to a plagiarism-checking service,
//  which may retain a copy of the project on its database.
//
//  Mover structure for integration
//  By D. Buckstein
//	This file was modified by Wednesday-David Hartman, Ty Wood with permission from author.





#include "Mover.h"





// initializer for the mover
// written by: David
Mover::Mover()
{
	init();
}

Mover::Mover(float dragCoeff, float surfArea)
:dragCoefficient(dragCoeff), surfaceArea(surfArea)
{
	init();
}

Mover::~Mover() {}

void Mover::init()
{
	// we're not a spring
	m_spring = nullptr;

	// we're born anew!
	modelMatrix = cbmath::m4Identity;

	// set our gravity
	accelerationFixed = cbmath::v3y * GRAVITATIONAL_CONSTANT;
}



// set the anchor for our spring so we don't bounce into infinity
// written by: David
void Mover::setSpringAnchor(cbmath::vec3 anchor)
{
	m_spring = new Spring(anchor);
}



// set the mass of this mover
// written by: Ty
void Mover::setMass(float mass)
{
	if (mass > 0.0f) massInverse = 1.0f / (this->mass = mass);
	else             massInverse = this->mass = 0.0f;
}



// add a force to this mover
// written by: Ty
void Mover::addForce(const cbmath::vec3 force)
{
	this->force += force;
}



// calculate drag for this mover
// written by: Wednesday-David
cbmath::vec3 Mover::calculateDrag(const float fluidDensity)
{
	const float velNormalizedSq = cbmath::lengthSq(velocity);
	if (velNormalizedSq > 0.0f)
	{
		// f = (c*d*a*v^2)/2
		const float drag = (dragCoefficient * fluidDensity * surfaceArea * velNormalizedSq) * 0.5f;
		const float newMagnitude = -drag / sqrt(velNormalizedSq);
		const cbmath::vec3 forceDrag = newMagnitude * velocity;
		return forceDrag;
	}
	return cbmath::v3zero;
}




// physics (integration) updates
// written by: Ty
void Mover::updateVelocity(const float dt)
{
	// v = v0 + a*dt
	velocity += acceleration * dt;
}


void Mover::updateAcceleration()
{
	// a = F/m = F * im
	acceleration = (force * massInverse) + (accelerationFixed);

	force.set();
}


void Mover::updateMoverDisplacement(const float dt)
{
	// x = x0 + v * dt + 0.5 * a * dt^2
	position += velocity * dt + 0.5 * acceleration * (dt * dt);

	updateVelocity(dt);
	updateAcceleration();
}



// simple (super simple) collision with the ground plane
// written by: Ty and Wednesday-David
void Mover::clampMoverToGround(const float groundHeight, const float restitution)
{
	if (position.y < groundHeight)
	{
		// fix position
		position.y = groundHeight + (groundHeight - position.y) * restitution;

		// fix velocity using reflection vector
		// reflectionVector = v - (2*dot(N, v)*N)
		cbmath::vec3 v = velocity;
		cbmath::vec3 N = cbmath::v3y;
		cbmath::vec3 reflVec = v - (2 * cbmath::dot(N, v) * N);
		velocity = reflVec * restitution;
	}
}



// graphics updates
// written by: Ty
void Mover::updateMoverGraphics()
{
	modelMatrix.c3.xyz = position;
}
