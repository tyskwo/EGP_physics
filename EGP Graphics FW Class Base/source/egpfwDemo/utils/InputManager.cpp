#include "InputManager.h"

#include "egpfw\egpfw\utils\egpfwInputUtils.h"

wh::InputManager::InputManager()
:m_dataFileSelection(1)
{
	m_currentParameterSettings = { wh::ParameterOptions::COLOR, wh::ParameterSuboptions::X, wh::ParameterType::VALUE };
}

wh::InputManager::~InputManager()
{
}




void wh::InputManager::prepareForSave(int dataFileSelection)
{
	m_saveFlag = true;
	m_dataFileSelection = dataFileSelection;
}

void wh::InputManager::prepareForLoad(int dataFileSelection)
{
	m_loadFlag = true;
	m_dataFileSelection = dataFileSelection;
}




void wh::InputManager::update(egpKeyboard *keybd)
{
	if (egpKeyboardIsKeyDown(keybd, 'z'))
	{
		// save
		if (egpKeyboardIsKeyPressed(keybd, '1'))
			prepareForSave(1);
		else if (egpKeyboardIsKeyPressed(keybd, '2'))
			prepareForSave(2);
		else if (egpKeyboardIsKeyPressed(keybd, '3'))
			prepareForSave(3);
		else if (egpKeyboardIsKeyPressed(keybd, '4'))
			prepareForSave(4);
	}
	else if (egpKeyboardIsKeyDown(keybd, 'x'))
	{
		// load
		if (egpKeyboardIsKeyPressed(keybd, '1'))
			prepareForLoad(1);
		else if (egpKeyboardIsKeyPressed(keybd, '2'))
			prepareForLoad(2);
		else if (egpKeyboardIsKeyPressed(keybd, '3'))
			prepareForLoad(3);
		else if (egpKeyboardIsKeyPressed(keybd, '4'))
			prepareForLoad(4);
	}
	else
	{

	}
}