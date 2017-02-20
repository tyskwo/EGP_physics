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





#include "ParticleSystem.h"

#include <stdlib.h>





ParticleSystem::ParticleSystem() {}



// initialize this system with the given skeleton
// written by: Ty
ParticleSystem::ParticleSystem(Particle::Data particleData,
                               Emitter::Mode  mode,
                               cbmath::vec3   position,
                               cbmath::vec3   emitDirection,
                               int            numberToEmit)
{
    // initialize the mover, set its position
	m_mover = new Mover();
    this->m_mover->position         = position;

    // set our particle data and its position
    this->m_particleData            = particleData;
    m_particleData.position         = position;
    
    // set up our emitter
    this->m_emitter.m_mode          = mode;
    this->m_emitter.m_emitDirection = emitDirection;
    this->m_emitter.m_numberToEmit  = numberToEmit;
}




// set the particle data to a new Particle::Data
// written by: Ty
void ParticleSystem::setParticleData(Particle::Data data)
{
	data.position = m_mover->position;
	m_particleData = data;
}



// called every frame
// written by: Ty
void ParticleSystem::update(const float dt)
{
    // iterate through all of our particles
    for(int i = 0; i < m_particles.size(); i++)
    {
        Particle *p = m_particles.at(i);
        
        // update the particle
        p->update(dt);
        
        // if the particle is dead, remove the reference to it
        if(!p->isAlive())
        {
            m_particles.erase(m_particles.begin() + i);
            i--;
        }
    }
}




// called when we need to create new particles
// written by: Ty
void ParticleSystem::emit(Model* model)
{
    // create new particles
    for(int i = 0; i < m_emitter.m_numberToEmit; i++)
    {
        m_particles.push_back(new Particle(m_particleData));

        m_particles.back()->setModel(model);
    }
}




// render the system when called
// written by: Ty
void ParticleSystem::render(cbmath::mat4 viewProjMatrix, cbmath::vec4 cameraPos)
{
    // render each of our particles
    for(Particle *p : m_particles) { p->render(viewProjMatrix, cameraPos); }
}
