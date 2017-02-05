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



#include "Particle.h"



Particle::Particle() {}



Particle::Particle(cbmath::vec3 position, cbmath::vec3 velocity = cbmath::vec3(0.0f,0.0f,0.0f), float mass = 1.0f, float lifespan = 1.0f)
{
    // set the physics values
    m_mover->position = position;
	m_mover->velocity = velocity;
	m_mover->mass     = mass;
    
    // set the lifespan values
    this->m_lifespan = lifespan;
    this->m_currentLife = 0.0f;
    
    // this particle is now alive
    this->m_isActive = true;
}




void Particle::update(const float dt)
{
    updateMoverDisplacement(this->m_mover, dt);
}



void Particle::render()
{
    
}
