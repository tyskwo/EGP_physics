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





#include "Locator.h"
#include "SaveManager.h"




// set the service to null to start
SaveManager *Locator::m_service = nullptr;




// getter for the save manager
SaveManager *Locator::getSaveManager()
{
	return m_service;
}




// setter for the save manager
void Locator::provide(SaveManager *service)
{
	m_service = service;
}




// delete the save manager on clean up
void Locator::cleanup()
{
	if (m_service != nullptr)
	{
		delete m_service;
		m_service = nullptr;
	}
}
