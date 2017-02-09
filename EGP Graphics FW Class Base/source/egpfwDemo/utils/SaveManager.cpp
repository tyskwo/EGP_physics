//
//  SaveManager.cpp
//  egpfwDemo
//
//  Created by Wednesday-David Hartman on 2/8/17.
//  Copyright © 2017 Dan Buckstein. All rights reserved.
//

#include "SaveManager.h"

#include <iostream>
#include <fstream>

SaveManager::SaveManager(std::string dataFilePath)
:m_dataFilePath(dataFilePath)
{
}

SaveManager::~SaveManager()
{
}

void SaveManager::loadData()
{
	// read data type
	// read in variable values based on type
	// assign values to a ParticleData struct
	std::cout << std::endl << m_dataFilePath << std::endl;

	std::ifstream ifs;

	ifs.open(m_dataFilePath);
	if (ifs.is_open())
	{
		std::string line;

		std::string type;
		std::string name;
		std::string data;
		std::string empty;

		while (getline(ifs, line))
		{
			// read in type, name, and data as strings
			type = line;
			getline(ifs, name);
			getline(ifs, data);
			getline(ifs, empty);
			

			// parse data based on type
			if (type == "vec3")
			{
				float vec3values[3] = { 0, 0, 0 };
				int j = 0;
				// iterate through the data string to get the actual values
				for (unsigned int i = 0; i < data.size(); i++)
				{
					if (data[i] != ' ')
					{
						// if this character in the string isn't a space, convert it from a char to a float
						vec3values[j] = static_cast<float>(data[i] - '0');
						j++;
					}
				}

				// assign the data values to an actual vec3
				cbmath::vec3 vec = cbmath::vec3(vec3values[0], vec3values[1], vec3values[2]);
				m_mapVec3.insert(std::pair<std::string, cbmath::vec3>(name, vec));
			}
			else if (type == "vec4")
			{
				float vec4values[4] = { 0, 0, 0, 0 };
				int j = 0;
				// iterate through the data string to get the actual values
				for (unsigned int i = 0; i < data.size(); i++)
				{
					if (data[i] != ' ')
					{
						// if this character in the string isn't a space, convert it from a char to a float
						vec4values[j] = static_cast<float>(data[i] - '0');
						j++;
					}
				}

				// assign the data values to an actual vec3
				cbmath::vec4 vec = cbmath::vec4(vec4values[0], vec4values[1], vec4values[2], vec4values[3]);
				m_mapVec3.insert(std::pair<std::string, cbmath::vec3>(name, vec));
			}
			else if (type == "float")
			{
				m_mapFloat.insert(std::pair<std::string, float>(name, atoi(data.c_str())));
			}
			else if (type == "bool")
			{
				m_mapBool.insert(std::pair<std::string, bool>(name, data == "0" ? false : true));
			}
			else if (type == "char")
			{
				m_mapChar.insert(std::pair<std::string, char>(name, data[0]));
			}
		}
	}
	else
	{
		std::cout << std::endl << "ERROR: INVALID PATH" << std::endl;
	}
	ifs.close();
}

void SaveManager::saveData()
{
}

void SaveManager::printData()
{
	std::cout << m_mapVec3["position"].x;
}



template <typename T>
void SaveManager::getData(std::string name)
{
	std::cout << name;
}

template <>
void SaveManager::getData<float>(std::string name)
{
	std::cout << m_mapFloat[name];
}