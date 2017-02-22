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




// initialize given a data to model
// written by: Ty
Particle::Particle(Data data)
{
    // instantiate a mover
	m_mover = new Mover(0.5f, 6.3f); // surface area of icosahedron = 5 * (edgeLength)^2 * sqrt(3)

    // set the physics values
    m_mover->position = data.position;
    m_mover->velocity = data.velocity.init();
	m_mover->setMass(data.mass.init());
    
    // set the color
    this->m_color = data.color;
    
    // set the lifespan values
    this->m_lifespan    = data.lifespan.init();
    this->m_currentLife = 0.0f;

	// set restitution
	this->m_restitution = data.restitution.init();
    
    // this particle is now alive
    this->m_isActive = true;
    
    
	m_springInitialized = false;
}



// delete this particle's mover
// written by: Wednesday-David
Particle::~Particle()
{
	delete m_mover;
	m_mover = nullptr;
}



// called ever frame
// written by: Ty
void Particle::update(const float dt)
{
    // if this particle is alive...
    if(m_isActive)
    {
		// add drag
		cbmath::vec3 drag = this->m_mover->calculateDrag(1.293f); //fluid density of atmosphere at 0C is 1.293kg/m^3
		this->m_mover->addForce(drag);

        // update our position
        this->m_mover->updateMoverDisplacement(dt);
		
        // perform basic collision with a given restitution
		this->m_mover->clampMoverToGround(0.0f, m_restitution);
        
        // add to our age
        m_currentLife += dt;
        
        
        // if we're alive longer than our lifespan
        if(m_currentLife > m_lifespan)
        {
            // kill us
            m_isActive = false;
            return;
        }
    
        //lerp our color
        m_color.current = m_color.lerp(m_currentLife / m_lifespan);
    }
}





// called when the game wants us to render
// written by: Ty
void Particle::render(cbmath::mat4 viewProjMatrix, cbmath::vec4 cameraPos)
{
    // if this particle is alive...
    if(m_isActive)
    {
        // update our graphics position
		this->m_mover->updateMoverGraphics();
        
        // and render the model
        this->m_model->renderAt(viewProjMatrix * this->m_mover->modelMatrix, cameraPos, m_color.current);
    }
}
