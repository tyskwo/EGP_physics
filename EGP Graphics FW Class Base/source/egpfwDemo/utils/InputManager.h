//
//  InputManager.h
//  egpfwDemo
//
//  Created by Wednesday-David Hartman on 2/16/17.
//  Copyright 2017 Dan Buckstein. All rights reserved.
//

#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H

namespace wh
{
	enum ParameterOptions
	{
		COLOR,
		VELOCITY,
		LIFESPAN,
		MASS
	};

	enum ParameterSuboptions
	{
		NONE,
		X, Y, Z, W
	};

	enum ParameterType
	{
		VALUE,
		DELTA
	};
}


#endif // __INPUTMANAGER_H