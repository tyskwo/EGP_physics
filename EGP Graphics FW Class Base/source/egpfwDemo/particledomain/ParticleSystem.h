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
    
    
private:
    
    // the particle this system will be emitting
    Particle modelParticle;
    
    // the list of particles this system is responsible for
    std::vector<Particle> particles;
    
    // the mover this particle system uses
    egpfwMover *mover;
    
    
    
public:
    
    ParticleSystem();
    ParticleSystem(cbmath::vec3 position, 
    
    inline Particle              getModelParticle() { return modelParticle; }
    inline std::vector<Particle> getParticles()     { return particles;     }
    
    void update(const float dt);
    
    void emit();
    
    void render();
    
};



#endif /* ParticleSystem_h */
