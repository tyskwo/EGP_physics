//  Wednesday-David Hartman 0923439
//  Ty Wood					0901793
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

#ifdef _WIN32
	#include "..\particledomain\Particle.h"
	#include "..\Model.h"
#else
	#include "Particle.h"
	#include "Model.h"
#endif





// a class that holds the logic of spawning and destroying a system of particles
// written by: Ty
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
    Particle::Data m_particleData;
    
    // the list of particles this system is responsible for
    std::vector<Particle*> m_particles;
    
    // the mover this particle system uses
    Mover *m_mover;
    
    // the emitter this particle system uses
    Emitter m_emitter;
    
    
    
    
 
public:
    
    // initializers
    ParticleSystem();
    ParticleSystem(Particle::Data particleData,
                   Emitter::Mode  mode           = Emitter::Mode::Burst,
                   cbmath::vec3   position       = cbmath::vec3(0.0f,0.0f,0.0f),
                   cbmath::vec3   emitDirection  = cbmath::vec3(0.0f,1.0f,0.0f),
                   int            numberToEmit   = 100);

    
    
    // function that sets the particle data
	void setParticleData(Particle::Data data);
    
    // function that updates this system's position
    void updatePositionDelta(cbmath::vec3 delta);

    
    
    // getters for the particle data and the particles we are responsible for
    inline Particle::Data         getParticleData()	{ return m_particleData; }
    inline std::vector<Particle*> getParticles()	{ return m_particles;    }
    
    // getter for what type of emitter we are using
    inline bool isContinuous()    { return m_emitter.m_mode == Emitter::Continuous; }
    
    // getter for the mover
    inline Mover* getMover() { return m_mover; }
    
    
    
    // called every frame
    void update(const float dt);
    
    
    
    // called when we're suppose to create new particles
    void emit(Model* model);
    
    
    
    // called when the game wants us on screen
    void render(cbmath::mat4 viewProjMatrix, cbmath::vec4 cameraPos);
};





#endif /* ParticleSystem_h */
