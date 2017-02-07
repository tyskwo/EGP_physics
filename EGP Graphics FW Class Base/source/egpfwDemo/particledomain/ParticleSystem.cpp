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

ParticleSystem::ParticleSystem(Particle::Data particleData,
                               Emitter::Mode  mode,
                               cbmath::vec3   position,
                               cbmath::vec3   emitDirection,
                               int            numberToEmit)
{
	m_mover = new Mover();
    this->m_mover->position         = position;

    this->m_particleData            = particleData;
    m_particleData.position         = position;
    
    this->m_emitter.m_mode          = mode;
    this->m_emitter.m_emitDirection = emitDirection;
    this->m_emitter.m_numberToEmit  = numberToEmit;
}



void ParticleSystem::update(const float dt)
{
    for(int i = 0; i < m_particles.size(); i++)
    {
        Particle *p = m_particles.at(i);
        
        p->update(dt);
        
        if(!p->isAlive())
        {
            m_particles.erase(m_particles.begin() + i);
            i--;
        }
    }
}



void ParticleSystem::emit(Model* model)
{
    for(int i = 0; i < m_emitter.m_numberToEmit; i++)
    {
        m_particles.push_back(new Particle(m_particleData));
        
        m_particles.back()->SetModel(model);
    }
}



void ParticleSystem::render(cbmath::mat4 viewProjMatrix)
{
    for(Particle *p : m_particles) { p->render(viewProjMatrix); }
}
