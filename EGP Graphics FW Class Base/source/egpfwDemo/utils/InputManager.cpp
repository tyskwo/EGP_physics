#include "InputManager.h"

#include <iostream>

#ifdef _WIN32
    #include "egpfw\egpfw\utils\egpfwInputUtils.h"
#else
    #include "egpfw/egpfw/utils/egpfwInputUtils.h"
#endif

#include "cbmath/cbtkMatrix.h"

#include "Locator.h"
#include "SaveManager.h"

wh::InputManager::InputManager()
:m_dataFileSelection(1), m_currentDisplayIndex(0)
{
	m_displayOptions.push_back("color");
	m_displayOptions.push_back("velocity");
	m_displayOptions.push_back("lifespan");
	m_displayOptions.push_back("mass");

	m_currentParameterSettings = { wh::ParameterOptions::COLOR, wh::ParameterSuboptions::X, wh::ParameterType::VALUE };
}

wh::InputManager::~InputManager()
{
}



void wh::InputManager::initiateSave(int dataFileSelection)
{
	m_dataFileSelection = dataFileSelection;
	Locator::getSaveManager()->saveData(m_dataFileSelection);
	std::cout << "saved current data to file " << m_dataFileSelection << std::endl;
}

void wh::InputManager::initiateLoad(int dataFileSelection)
{
	m_dataFileSelection = dataFileSelection;
	Locator::getSaveManager()->loadData(m_dataFileSelection);
	std::cout << "loaded data from file " << m_dataFileSelection << std::endl;
}



void wh::InputManager::setParameterOption(wh::ParameterOptions option)
{
	m_currentDisplayIndex = (int)option;
	m_displayFlag = true;

	m_currentParameterSettings.m_parameterOption = option;
	switch (m_currentParameterSettings.m_parameterOption)
	{
	case wh::ParameterOptions::COLOR:
	case wh::ParameterOptions::VELOCITY:
		m_currentParameterSettings.m_parameterSuboption = wh::ParameterSuboptions::X;
		break;
	case wh::ParameterOptions::LIFESPAN:
	case wh::ParameterOptions::MASS:
		m_currentParameterSettings.m_parameterSuboption = wh::ParameterSuboptions::NONE;
		break;
	default:
		break;
	}
}

void wh::InputManager::setParameterSuboption(wh::ParameterSuboptions suboption)
{
	bool isValidVec3 = (suboption == wh::ParameterSuboptions::X || suboption == wh::ParameterSuboptions::Y || suboption == wh::ParameterSuboptions::Z);
	bool isValidVec4 = (isValidVec3 || suboption == wh::ParameterSuboptions::W);

	switch (m_currentParameterSettings.m_parameterOption)
	{
	case wh::ParameterOptions::COLOR:
		if (isValidVec4)
		{
			m_currentParameterSettings.m_parameterSuboption = suboption;
			m_displayFlag = true;
		}
	case wh::ParameterOptions::VELOCITY:
		if (isValidVec3)
		{
			m_currentParameterSettings.m_parameterSuboption = suboption;
			m_displayFlag = true;
		}
		break;
	case wh::ParameterOptions::LIFESPAN:
	case wh::ParameterOptions::MASS:
		break;
	default:
		break;
	}
}



void wh::InputManager::update(egpKeyboard *keybd, egpMouse *mouse, int windowWidth)
{
	handleKeyboardInput(keybd);
	handleMouseInput(mouse, windowWidth);

	if (m_displayFlag)
	{
		m_displayFlag = false;
		display();
	}
}

void wh::InputManager::handleKeyboardInput(egpKeyboard *keybd)
{
	if (egpKeyboardIsKeyDown(keybd, 'z'))
	{
		// save
		if (egpKeyboardIsKeyPressed(keybd, '1'))
			initiateSave(1);
		else if (egpKeyboardIsKeyPressed(keybd, '2'))
			initiateSave(2);
		else if (egpKeyboardIsKeyPressed(keybd, '3'))
			initiateSave(3);
		else if (egpKeyboardIsKeyPressed(keybd, '4'))
			initiateSave(4);
	}
	else if (egpKeyboardIsKeyDown(keybd, 'x'))
	{
		// load
		if (egpKeyboardIsKeyPressed(keybd, '1'))
			initiateLoad(1);
		else if (egpKeyboardIsKeyPressed(keybd, '2'))
			initiateLoad(2);
		else if (egpKeyboardIsKeyPressed(keybd, '3'))
			initiateLoad(3);
		else if (egpKeyboardIsKeyPressed(keybd, '4'))
			initiateLoad(4);
	}
	else
	{
		// select parameter to adjust
		if (egpKeyboardIsKeyPressed(keybd, 'c'))
			setParameterOption(wh::ParameterOptions::COLOR);
		else if (egpKeyboardIsKeyPressed(keybd, 'v'))
			setParameterOption(wh::ParameterOptions::VELOCITY);
		else if (egpKeyboardIsKeyPressed(keybd, 'l'))
			setParameterOption(wh::ParameterOptions::LIFESPAN);
		else if (egpKeyboardIsKeyPressed(keybd, 'm'))
			setParameterOption(wh::ParameterOptions::MASS);

		// select parameter suboption
		if (egpKeyboardIsKeyPressed(keybd, '1'))
			setParameterSuboption(wh::ParameterSuboptions::X);
		else if (egpKeyboardIsKeyPressed(keybd, '2'))
			setParameterSuboption(wh::ParameterSuboptions::Y);
		else if (egpKeyboardIsKeyPressed(keybd, '3'))
			setParameterSuboption(wh::ParameterSuboptions::Z);
		else if (egpKeyboardIsKeyPressed(keybd, '4'))
			setParameterSuboption(wh::ParameterSuboptions::W);

		// select delta
		if (egpKeyboardIsKeyPressed(keybd, 'g'))
		{
			if (m_currentParameterSettings.m_parameterType == wh::ParameterType::VALUE)
				m_currentParameterSettings.m_parameterType = wh::ParameterType::DELTA;
			else
				m_currentParameterSettings.m_parameterType = wh::ParameterType::VALUE;

			m_displayFlag = true;
		}
	}
}

void wh::InputManager::handleMouseInput(egpMouse *mouse, int windowWidth)
{
	// handle adjustment of parameters
	if (egpMouseIsButtonDown(mouse, 2))
	{
		// set the desired range to map (scaleClamp) the mouse x position onto
		float clampMin = 0.0f;
		float clampMax = 1.0f;

		switch (m_currentParameterSettings.m_parameterOption)
		{
		case wh::ParameterOptions::COLOR:
			clampMin = 0.0f;
			clampMax = 1.0f;
			break;
		case wh::ParameterOptions::VELOCITY:
			clampMin = -10.0f;
			clampMax = 10.0f;
			break;
		case wh::ParameterOptions::LIFESPAN:
			clampMin = 0.0f;
			clampMax = 10.0f;
			break;
		case wh::ParameterOptions::MASS:
			clampMin = 0.0f;
			clampMax = 10.0f;
			break;
		default:
			break;
		}

		float clampedDeltaX = scaleClamp((float)egpMouseX(mouse), 0.0f, (float)windowWidth, clampMin, clampMax);
		std::cout << clampedDeltaX;


		// adjusted the selected parameter
		if (m_currentDisplayIndex == 0)
		{
			// color
			std::string colorVarName = ((m_currentParameterSettings.m_parameterType == wh::ParameterType::VALUE) ? "colorStart" : "colorEnd");
			cbmath::vec4 color = Locator::getSaveManager()->getData<cbmath::vec4>(colorVarName);

			switch (m_currentParameterSettings.m_parameterSuboption)
			{
			case wh::ParameterSuboptions::NONE:
				break;
			case wh::ParameterSuboptions::X:
				Locator::getSaveManager()->setData<cbmath::vec4>(colorVarName, cbmath::v4x * clampedDeltaX + cbmath::v4y * color.y + cbmath::v4z * color.z + cbmath::v4w * color.w);
				break;
			case wh::ParameterSuboptions::Y:
				Locator::getSaveManager()->setData<cbmath::vec4>(colorVarName, cbmath::v4x * color.x + cbmath::v4y * clampedDeltaX + cbmath::v4z * color.z + cbmath::v4w * color.w);
				break;
			case wh::ParameterSuboptions::Z:
				Locator::getSaveManager()->setData<cbmath::vec4>(colorVarName, cbmath::v4x * color.x + cbmath::v4y * color.y + cbmath::v4z * clampedDeltaX + cbmath::v4w * color.w);
				break;
			case wh::ParameterSuboptions::W:
				Locator::getSaveManager()->setData<cbmath::vec4>(colorVarName, cbmath::v4x * color.x + cbmath::v4y * color.y + cbmath::v4z * color.z + cbmath::v4w * clampedDeltaX);
				break;
			default:
				break;
			}
		}
		else if (m_currentDisplayIndex == 1)
		{
			// velocity
			std::string velocityVarName = ((m_currentParameterSettings.m_parameterType == wh::ParameterType::VALUE) ? "velocityValue" : "velocityDelta");
			cbmath::vec3 vel = Locator::getSaveManager()->getData<cbmath::vec3>(velocityVarName);

			switch (m_currentParameterSettings.m_parameterSuboption)
			{
			case wh::ParameterSuboptions::NONE:
				break;
			case wh::ParameterSuboptions::X:
				Locator::getSaveManager()->setData<cbmath::vec3>(velocityVarName, cbmath::v3x * clampedDeltaX + cbmath::v3y * vel.y + cbmath::v3z * vel.z);
				break;
			case wh::ParameterSuboptions::Y:
				Locator::getSaveManager()->setData<cbmath::vec3>(velocityVarName, cbmath::v3x * vel.x + cbmath::v3y * clampedDeltaX + cbmath::v3z * vel.z);
				break;
			case wh::ParameterSuboptions::Z:
				Locator::getSaveManager()->setData<cbmath::vec3>(velocityVarName, cbmath::v3x * vel.x + cbmath::v3y * vel.y + cbmath::v3z * clampedDeltaX);
				break;
			default:
				break;
			}
		}
		else if (m_currentDisplayIndex == 2)
		{
			// lifespan
			std::string lifespanVarName = ((m_currentParameterSettings.m_parameterType == wh::ParameterType::VALUE) ? "lifespanValue" : "lifespanDelta");
			Locator::getSaveManager()->setData<float>(lifespanVarName, clampedDeltaX);
		}
		else if (m_currentDisplayIndex == 3)
		{
			// mass
			std::string massVarName = ((m_currentParameterSettings.m_parameterType == wh::ParameterType::VALUE) ? "massValue" : "massDelta");
			Locator::getSaveManager()->setData<float>(massVarName, clampedDeltaX);
		}
	}
}



void wh::InputManager::display()
{
	std::cout << std::endl << m_displayOptions[m_currentDisplayIndex] <<
		(m_currentParameterSettings.m_parameterOption == wh::ParameterOptions::COLOR ?
		(m_currentParameterSettings.m_parameterType == wh::ParameterType::VALUE ? " start " : " end ") :
		(m_currentParameterSettings.m_parameterType == wh::ParameterType::VALUE ? " value " : " delta ")) << ", ";

	switch (m_currentParameterSettings.m_parameterSuboption)
	{
	case wh::ParameterSuboptions::NONE:
		break;
	case wh::ParameterSuboptions::X:
		m_currentParameterSettings.m_parameterOption == wh::ParameterOptions::VELOCITY ? std::cout << "x" : std::cout << "r";
		break;
	case wh::ParameterSuboptions::Y:
		m_currentParameterSettings.m_parameterOption == wh::ParameterOptions::VELOCITY ? std::cout << "y" : std::cout << "g";
		break;
	case wh::ParameterSuboptions::Z:
		m_currentParameterSettings.m_parameterOption == wh::ParameterOptions::VELOCITY ? std::cout << "z" : std::cout << "b";
		break;
	case wh::ParameterSuboptions::W:
		m_currentParameterSettings.m_parameterOption == wh::ParameterOptions::VELOCITY ? std::cout << "w" : std::cout << "a";
		break;
	default:
		break;
	}

	std::cout << " ";
}



float wh::InputManager::scaleClamp(float value, float min, float max, float min2, float max2)
{
	value = min2 + ((value - min) / (max - min)) * (max2 - min2);
	if (max2 > min2)
	{
		value = value < max2 ? value : max2;
		return value > min2 ? value : min2;
	}
	value = value < min2 ? value : min2;
	return value > max2 ? value : max2;
}
