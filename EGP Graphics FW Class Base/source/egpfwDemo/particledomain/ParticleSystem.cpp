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

ParticleSystem::ParticleSystem(Particle*     modelParticle,
                               Emitter::Mode mode,
                               cbmath::vec3  position,
                               cbmath::vec3  emitDirection,
                               int           numberToEmit)
{
	m_mover = new Mover();
    this->m_modelParticle           = modelParticle;
    this->m_emitter.m_mode          = mode;
    this->m_emitter.m_emitDirection = emitDirection;
    this->m_emitter.m_numberToEmit  = numberToEmit;
    this->m_mover->position         = position;
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



void ParticleSystem::emit()
{
    for(int i = 0; i < m_emitter.m_numberToEmit; i++)
    {
        m_particles.push_back(new Particle(m_mover->position, m_emitter.m_emitDirection + cbmath::v3x * (static_cast<float>(rand()) / RAND_MAX) + cbmath::v3y * (static_cast<float>(rand()) / RAND_MAX) + cbmath::v3z * (static_cast<float>(rand()) / RAND_MAX), 0.5f, 5.0f));
    }
}



void ParticleSystem::render(cbmath::mat4 viewProjMatrix)
{
    for(Particle *p : m_particles)
    {
        p->render(viewProjMatrix);
    }
}
