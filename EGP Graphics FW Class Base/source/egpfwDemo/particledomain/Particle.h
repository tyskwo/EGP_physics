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





#ifndef Particle_h
#define Particle_h





#include <stdio.h>

#include "egpfw/egpfw.h"

#ifdef _WIN32
	#include "..\gphysics\Mover.h"
	#include "..\Model.h"
	#include "..\Utils.h"
	#include "..\Eases.h"
#else
	#include "Mover.h"
    #include "Model.h"
	#include "Utils.h"
	#include "Eases.h"
#endif





// class that defines a particle, its data, and everything else about it
// written by: Ty
class Particle
{
    
public:
    
    // a struct that defines a property that is a delta type
    template <typename T>
    struct DeltaType
    {
        // the base value and the total delta
        T value, delta;
        
        //whether or not the value is a midpoint
        bool isDeltaMidpoint;
        
        // initializer
        T init()
        {
            // return the appropriate value
            if(isDeltaMidpoint)
            {
                return value + Utils::randomDeltaPosNeg(delta);
            }
            else
            {
                return value + Utils::randomDelta(delta);
            }
        }
    };
    
    
    
    // a struct that defines a property that is an over-lifetime type
    template <typename T>
    struct LifetimeType
    {
        // typedef for timing function
        typedef float (*TimingFunction)(float);
        
        // the start value and the end value
        T start, end;
        
        // the timing function to lerp with
        TimingFunction ease;
        
        // the current value
        T current;
        
        // lerp to the given percent give all of our current values
        T lerp(float percent)
        {
            return Eases::lerp(start, end, percent, ease);
        }
    };
    
    // a struct holding all of the data that defines this particle
    struct Data
    {
        cbmath::vec3 position;
        
        DeltaType<float>        lifespan;
        DeltaType<float>        mass;
        DeltaType<cbmath::vec3> velocity;

        LifetimeType<cbmath::vec4> color;
        LifetimeType<cbmath::vec3> scale;
    };
    

    
private:
    
    // reference to the mover struct of this particle for physics simulation
    Mover *m_mover;
    
    // how long this particle lives for
    float m_lifespan;
    
    // how long this particle has been alive
    float m_currentLife;
    
    // whether or not this particle is dead
    bool  m_isActive;
    
	bool  m_springInitialized;

    // the model that this particle uses to render
    Model* m_model;
    
    // the current color of this particle
    LifetimeType<cbmath::vec4> m_color;
    
    
    
public:
    
    Particle();
    Particle(Data data);
	~Particle();
    
    
    // getters for the mover attached to this particle and whether or not this particle is alive
    inline Mover* getMover() { return m_mover;    }
    inline bool   isAlive()  { return m_isActive; }
    
    
    
    // called every frame
    void update(const float dt);
    
    
    
    // called when we want to render this particle
    void render(cbmath::mat4 viewProjMatrix, cbmath::vec4 cameraPos);
    
    
    
    // sets the particle's model
    inline void setModel(Model* model) { m_model = model; }
};





#endif /* Particle_h */
