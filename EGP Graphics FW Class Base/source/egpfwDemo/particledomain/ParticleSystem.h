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



#ifndef ParticleSystem_h
#define ParticleSystem_h



#include <stdio.h>
#include <vector>

#include "Particle.h"



class ParticleSystem
{
public:
	// struct to hold emission data
	struct Emitter
	{
		// how does this system emit its particles?
		enum Mode { Burst, Continuous };
		Mode m_mode;

		// how many particles this system emits each emit() call
		int m_numberToEmit;

		// the direction to emit from
		cbmath::vec3 m_emitDirection;
	};
    
private:
    
    // the particle this system will be emitting
    Particle m_modelParticle;
    
    // the list of particles this system is responsible for
    std::vector<Particle*> m_particles;
    
    // the mover this particle system uses
    Mover *m_mover;
    
    Emitter m_emitter;
    
 
public:
    
    ParticleSystem();
    ParticleSystem(Particle      modelParticle,
                   Emitter::Mode mode           = Emitter::Mode::Burst,
                   cbmath::vec3  position       = cbmath::vec3(0.0f,0.0f,0.0f),
                   cbmath::vec3  emitDirection  = cbmath::vec3(0.0f,1.0f,0.0f),
                   int           numberToEmit   = 100);

    inline Particle               getModelParticle() { return m_modelParticle; }
    inline std::vector<Particle*> getParticles()     { return m_particles;     }
    
    void update(const float dt);
    
    void emit();
    
    void render();
    
};



#endif /* ParticleSystem_h */
