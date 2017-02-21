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





#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H


#include <vector>
#include <string>

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
		MASS
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



	struct AdjustableParameter
	{
		ParameterOptions m_parameterOption;
		ParameterSuboptions m_parameterSuboption;
		ParameterType m_parameterType;
	};

	class InputManager
	{
	private:
		void initiateSave(int dataFileSelection);
		void initiateLoad(int dataFileSelection);

		void setParameterOption(ParameterOptions option);
		void setParameterSuboption(ParameterSuboptions suboption);
		
		void handleKeyboardInput(egpKeyboard *keybd);
		void handleMouseInput(egpMouse *mouse, int windowWidth);

		void display();

		float scaleClamp(float value, float min, float max, float min2, float max2);


		std::vector<std::string> m_displayOptions;
		int m_currentDisplayIndex;

		AdjustableParameter m_currentParameterSettings;

		bool m_displayFlag;
        bool m_isPathControlled = false;

		int m_dataFileSelection;

	public:
		InputManager();
		~InputManager();

		inline int getSaveFileSelection() const { return m_dataFileSelection; };
        inline bool isPathControlled() const { return m_isPathControlled; };

		void update(egpKeyboard *keybd, egpMouse *mouse, int windowWidth);
	};
}





#endif // __INPUTMANAGER_H
