#include "Mover.h"

void Mover::setMass(float mass)
{
	if (mass > 0.0f) massInverse = 1.0f / (this->mass = mass);
	else             massInverse = this->mass = 0.0f;
}

void Mover::setDamping(float damping)
{
	this->damping = damping;
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

	velocity *= powf(damping, dt);
}



void Mover::updateAcceleration()
{
	// a = F/m = F * im
	acceleration = (force + accelerationFixed) * massInverse;

	force.set();
}



void Mover::updateMoverDisplacement(const float dt)
{
	//x = x0 + v * dt + 0.5 * a * dt^2
	position += velocity * dt + 0.5 * acceleration * (dt * dt);

	updateVelocity(dt);
	updateAcceleration();
}





// graphics updates
void Mover::updateMoverGraphics()
{
	modelMatrix.c3.xyz = position;
}