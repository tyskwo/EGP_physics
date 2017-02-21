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

// written by: Wednesday-David





#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H





#include <map>
#include <vector>
#include <string>





// references to the keyboard and mouse struct objects
struct egpKeyboard;
struct egpMouse;





// a namespace for handling keyboard input parameter changes
// written by: Wednesday-David
namespace wh
{
    // holds what particle properties are editable
	enum ParameterOptions
	{
		COLOR = 0,
		VELOCITY,
		LIFESPAN,
		MASS,
        EASE
	};

    
    // holds what particle subproperties are editable
	enum ParameterSuboptions
	{
		NONE = 0,
		X, Y, Z, W
	};

    
    // holds if a particle property has a delta or not
	enum ParameterType
	{
		VALUE = 0,
		DELTA
	};


    // holds all of the info related to adjusting a parameter
	struct AdjustableParameter
	{
		ParameterOptions    m_parameterOption;
		ParameterSuboptions m_parameterSuboption;
		ParameterType       m_parameterType;
	};

    
    
    
    
    // a class that handles all logic related to input
	class InputManager
	{
        
	private:
        
        // loading and saving from/to files
		void initiateSave(int dataFileSelection);
		void initiateLoad(int dataFileSelection);

        // setting the value of a parameter
		void setParameterOption(ParameterOptions option);
		void setParameterSuboption(ParameterSuboptions suboption);
		
        // callbacks for handling keyboard and mouse input
		void handleKeyboardInput(egpKeyboard *keybd);
		void handleMouseInput(egpMouse *mouse, int windowWidth);

        // method for putting text into the console
		void display();

        // a map that holds the current display strings
		std::map<ParameterOptions, std::string> m_displayOptions;
        
        // the current parameter we are editing display string
        ParameterOptions m_currentDisplayOption;
        
        // the current editable parameter
		AdjustableParameter m_currentParameterSettings;

        // flag for whether we should output to the console
		bool m_displayFlag;
        
        // flag for whether the particle system is path controlled
        bool m_isPathControlled = false;

        // reference to what data file is currently loaded
		int m_dataFileSelection;

        
        
	public:
        
		InputManager();
		~InputManager();

        // getters for the current save file and whether or not the PS is path-controlled
		inline int  getSaveFileSelection() const { return m_dataFileSelection; };
        inline bool isPathControlled()     const { return m_isPathControlled;  };

        // update, with references to the keyboard and mouse. (window width is used for RMB input)
		void update(egpKeyboard *keybd, egpMouse *mouse, int windowWidth);
	};
};





#endif // __INPUTMANAGER_H
