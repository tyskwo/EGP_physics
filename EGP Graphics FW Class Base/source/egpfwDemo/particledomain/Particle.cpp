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



#include "Particle.h"

#include <stdlib.h> 

#ifdef _WIN32
    #include "egpfw\egpfw\utils\egpfwPrimitiveDataUtils.h"
#else
    #include "../../../include/egpfw/egpfw/utils/egpfwPrimitiveDataUtils.h"
#endif


Particle::Particle() {}



Particle::Particle(cbmath::vec3 position, cbmath::vec3 velocity, float mass, float lifespan)
{
	m_mover = new Mover();

    // set the physics values
    m_mover->position = position;
	m_mover->velocity = velocity;
	m_mover->setMass(mass);
	m_mover->setDamping(0.5f);
    
    // set the lifespan values
    this->m_lifespan = lifespan;
    this->m_currentLife = 0.0f;
    
    // this particle is now alive
    this->m_isActive = true;
}




void Particle::update(const float dt)
{
    if(m_isActive)
    {
        this->m_mover->updateMoverDisplacement(dt);
        
        m_currentLife += dt;
        
        if(m_currentLife > m_lifespan) m_isActive = false;
    }
}



void Particle::render(cbmath::mat4 viewProjMatrix)
{
    if(m_isActive)
    {
		this->m_mover->updateMoverGraphics();
        
        
        egpProgram temp = m_model->getShader()->getProgram();
        egpActivateProgram(&temp);
        
        
        egpSendUniformFloatMatrix(m_model->getShader()->getMVPUniform(), UNIF_MAT4, 1, 0, (viewProjMatrix * this->m_mover->modelMatrix).m);
        
        egpActivateVAO(m_model->getVAO());
        
        egpDrawActiveVAO();


		// random colors
		//egpDrawWireCubeImmediate((viewProjMatrix * this->m_mover->modelMatrix).m, 0, 0, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX);
    }
}
