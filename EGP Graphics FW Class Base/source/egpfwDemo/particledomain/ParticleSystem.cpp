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


ParticleSystem::ParticleSystem() {}

ParticleSystem::ParticleSystem(Particle      modelParticle,
                               Emitter::Mode mode           = Emitter::Mode::Burst,
                               cbmath::vec3  position       = cbmath::vec3(0.0f,0.0f,0.0f),
                               cbmath::vec3  emitDirection  = cbmath::vec3(0.0f,1.0f,0.0f),
                               int           numberToEmit   = 100)
{
    this->m_modelParticle           = modelParticle;
    this->m_emitter.m_mode          = mode;
    this->m_emitter.m_emitDirection = emitDirection;
    this->m_emitter.m_numberToEmit  = numberToEmit;
    this->m_mover->position         = position;
}



void ParticleSystem::update(const float dt)
{
    for(Particle *p : m_particles)
    {
        p->update(dt);
    }
}



void ParticleSystem::emit()
{
    for(int i = 0; i < m_emitter.m_numberToEmit; i++)
    {
        m_particles.push_back(new Particle());
    }
}



void ParticleSystem::render()
{
    for(Particle *p : m_particles)
    {
        p->render();
    }
}
