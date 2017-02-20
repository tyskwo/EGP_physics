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





#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H




// a namespace for handling keyboard input parameter changes
// written by: David
namespace wh
{
    // holds what particle properties are editable
	enum ParameterOptions
	{
		COLOR,
		VELOCITY,
		LIFESPAN,
		MASS
	};

    
    
    
    // holds what particle subproperties are editable
	enum ParameterSuboptions
	{
		NONE,
		X, Y, Z, W
	};

    
    
    
    // holds if a particle property has a delta or not
	enum ParameterType
	{
		VALUE,
		DELTA
	};
}





#endif // __INPUTMANAGER_H
