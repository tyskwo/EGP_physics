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
    
    
    this->m_color = data.color;
    
    
    // set the lifespan values
    this->m_lifespan = data.lifespan.init();
    this->m_currentLife = 0.0f;
    
    // this particle is now alive
    this->m_isActive = true;
    
	m_springInitialized = false;
}




void Particle::update(const float dt)
{
    if(m_isActive)
    {
        this->m_mover->updateMoverDisplacement(dt);
		
		if (this->m_mover->m_spring != nullptr)
		{
			if (!m_springInitialized)
			{
				this->m_mover->m_spring->updateSpring(this->m_mover->position);
				m_springInitialized = true;
			}
			else
			{
				// TODO: move this to mover?
				const float maxSpringStiffness = this->m_mover->m_spring->getSpringCoefficient(this->m_mover->mass, 0.005f);
				const float maxSpringDamping = this->m_mover->m_spring->getDampingCoefficient(this->m_mover->mass, 0.005f);
				const float springStiffness = 0.001f * maxSpringStiffness;
				const float springDamping = 0.5f * maxSpringDamping;
				this->m_mover->m_spring->updateSpring(this->m_mover->position);
				cbmath::vec3 newForce = this->m_mover->m_spring->getForceStiffSpringDamped(springStiffness, springDamping);
				this->m_mover->addForce(newForce);
			}
		}
		
		this->m_mover->clampMoverToGround(0.0f, 0.8f);
        
        m_currentLife += dt;
        
        if(m_currentLife > m_lifespan)
        {
            m_isActive = false;
            return;
        }
    
        m_color.current = m_color.lerp(m_currentLife / m_lifespan);
    }
}



void Particle::render(cbmath::mat4 viewProjMatrix, cbmath::vec4 cameraPos)
{
    if(m_isActive)
    {
		this->m_mover->updateMoverGraphics();
        this->m_model->renderAt(viewProjMatrix * this->m_mover->modelMatrix, cameraPos, m_color.current);
    }
}
