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





//  Source:
//	http://gameprogrammingpatterns.com/service-locator.html
//  writen by: David





#ifndef __LOCATOR_H
#define __LOCATOR_H




// reference to the SaveManager
class SaveManager;




// a class that finds and holds a reference to a SaveManager
class Locator
{
    
private:
    
    // reference to the save manager
	static SaveManager *m_service;
    
    
    
public:
    
    // getter for the save manager
	static SaveManager *getSaveManager();

    
    // connection method for the save manager
	static void provide(SaveManager *service);

    
    // called upon destruction to release the save manager
	static void cleanup();
};



#endif // __LOCATOR_H
