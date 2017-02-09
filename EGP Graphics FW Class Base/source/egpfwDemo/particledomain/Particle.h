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



class Particle
{
public:
    
    template <typename T>
    struct DeltaType
    {
        T value, delta;
        bool isDeltaMidpoint;
        
        T init()
        {
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
    
    template <typename T>
    struct LifetimeType
    {
        typedef float (*TimingFunction)(float);
        
        T start, end;
        TimingFunction ease;
        T current;
        
        T lerp(float percent)
        {
            return Eases::lerp(start, end, percent, ease);
        }
    };
    
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
    
    float m_lifespan;
    
    // how long this particle has been alive
    float m_currentLife;
    
    // whether or not this particle is dead
    bool  m_isActive;
    
    Model* m_model;
    
    LifetimeType<cbmath::vec4> m_color;
    
    
    
    
    
public:
    
    Particle();
    Particle(Data data);
    
    inline Mover* getMover() { return m_mover;    }
    inline bool   isAlive()  { return m_isActive; }
    
    void update(const float dt);
    
    void render(cbmath::mat4 viewProjMatrix, cbmath::vec4 cameraPos);
    
    inline void SetModel(Model* model) { m_model = model; }
};



#endif /* Particle_h */
