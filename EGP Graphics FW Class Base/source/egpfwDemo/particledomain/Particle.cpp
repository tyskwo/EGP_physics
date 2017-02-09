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
	#include "..\Eases.h"
	#include "..\Utils.h"
#else
    #include "../../../include/egpfw/egpfw/utils/egpfwPrimitiveDataUtils.h"
	#include "Eases.h"
	#include "Utils.h"
#endif




Particle::Particle() {}



Particle::Particle(Data data)
{
	m_mover = new Mover();

    // set the physics values
    m_mover->position = data.position;
    
    m_mover->velocity = data.velocity.init();

	m_mover->setMass(data.mass.init());
    
	m_mover->setDamping(0.5f);
    
    
    this->m_color = data.color;
    
    
    // set the lifespan values
    this->m_lifespan = data.lifespan.init();
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
        
        if(m_currentLife > m_lifespan)
        {
            m_isActive = false;
            return;
        }
    
        m_color.current = m_color.lerp(m_currentLife / m_lifespan);
    }
}



void Particle::render(cbmath::mat4 viewProjMatrix)
{
    if(m_isActive)
    {
		this->m_mover->updateMoverGraphics();
        this->m_model->renderAt(viewProjMatrix * this->m_mover->modelMatrix, m_color.current);
    }
}
