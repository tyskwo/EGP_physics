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



#ifndef Particle_h
#define Particle_h



#include <stdio.h>

#ifdef _WIN32
	#include "cbmath\cbtkMatrix.h"
	#include "..\gphysics\Mover.h"
#else
	#include "Mover.h"
#endif



class Particle
{
    
    
private:
    
    // reference to the mover struct of this particle for physics simulation
    Mover *m_mover;
    
    // how long this particle lives for
    float m_lifespan;
    
    // how long this particle has been alive
    float m_currentLife;
    
    // whether or not this particle is dead
    bool  m_isActive;
    
    
    
public:
    
    Particle();
    Particle(cbmath::vec3 position, cbmath::vec3 velocity = cbmath::v3zero, float mass = 1.0f, float lifespan = 1.0f);
    
    inline Mover* getMover() { return m_mover;    }
    inline bool   isAlive()  { return m_isActive; }
    
    void update(const float dt);
    
    void render();
};



#endif /* Particle_h */
