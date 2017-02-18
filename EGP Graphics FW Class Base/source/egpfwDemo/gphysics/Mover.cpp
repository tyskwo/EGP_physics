//  David Hartman 0923439
//  Ty Wood       0901793
//
//  EGP 425.01
//  Simple Particle Engine
//  17.02.05
//
//  We certify that this work is entirely our own.
//  The assessor of this project may reproduce this project and provide copies to other academic staff,
//  and/or communicate a copy of this project to a plagiarism-checking service,
//  which may retain a copy of the project on its database.



#include "Mover.h"

Mover::Mover()
{
	m_spring = nullptr;
	modelMatrix = cbmath::m4Identity;
	accelerationFixed = cbmath::v3y * GRAVITATIONAL_CONSTANT;
}

Mover::~Mover()
{
}




void Mover::setSpringAnchor(cbmath::vec3 anchor)
{
	m_spring = new Spring(anchor);
}


void Mover::setMass(float mass)
{
	if (mass > 0.0f) massInverse = 1.0f / (this->mass = mass);
	else             massInverse = this->mass = 0.0f;
}

void Mover::addForce(const cbmath::vec3 force)
{
	this->force += force;
}




// physics (integration) updates
void Mover::updateVelocity(const float dt)
{
	// v = v0 + a*dt
	velocity += acceleration * dt;

	//velocity *= powf(damping, dt);
}


void Mover::updateAcceleration()
{
	// a = F/m = F * im
	acceleration = (force * massInverse) + (accelerationFixed * mass);

	force.set();
}


void Mover::updateMoverDisplacement(const float dt)
{
	// x = x0 + v * dt + 0.5 * a * dt^2
	position += velocity * dt + 0.5 * acceleration * (dt * dt);

	updateVelocity(dt);
	updateAcceleration();
}


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
void Mover::updateMoverGraphics()
{
	modelMatrix.c3.xyz = position;
}