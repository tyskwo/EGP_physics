//
//  SaveManager.cpp
//  egpfwDemo
//
//  Created by Wednesday-David Hartman on 2/8/17.
//  Copyright 2017 Dan Buckstein. All rights reserved.
//

#include "SaveManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

// initialize SaveManager
// written by: Wednesday-David
SaveManager::SaveManager(std::string dataFilePath)
:m_dataFilePath(dataFilePath), m_dataFileSelected(1)
{
}

// make sure every file is up-to-date during cleanup
// written by: Wednesday-David
SaveManager::~SaveManager()
{
	for (int i = 0; i < NUM_SAVE_FILES; i++)
	{
		loadData(i + 1);
		saveData(i + 1);
	}
}




//-----------------------------------------------------------------------------
// loading data

// load data from the specified data file into the data maps
// written by: Wednesday-David
void SaveManager::loadData(int dataFileSelected)
{
	m_dataFileSelected = dataFileSelected;

	std::ifstream ifs;

	std::stringstream filePath;
	filePath << m_dataFilePath << "data" << m_dataFileSelected << ".txt";

	ifs.open(filePath.str());
	if (ifs.is_open())
	{
		std::string line;

		std::string type;
		std::string name;
		std::string data;
		std::string empty;

		// while the endoffile has not been reached
		while (getline(ifs, line))
		{
			// read in type, name, and data as strings
			type = line;
			getline(ifs, name);
			getline(ifs, data);
			getline(ifs, empty);
			
			// parse data based on type
			if (type == "vec3")
				parseVec3(name, data);
			else if (type == "vec4")
				parseVec4(name, data);
			else if (type == "float")
				m_mapFloat[name] = std::stof(data.c_str());
			else if (type == "int")
				m_mapInt[name] = atoi(data.c_str());
			else if (type == "bool")
				m_mapBool[name] = data == "0" ? false : true;
			else if (type == "char")
				m_mapChar[name] = data[0];
		}
	}
	else
	{
		std::cout << std::endl << "ERROR: INVALID PATH" << std::endl;
	}
	ifs.close();
}

// parse a string into a vec3
// written by: Wednesday-David
void SaveManager::parseVec3(std::string name, std::string data)
{	
	float vec3values[3] = { 0, 0, 0 };
	int j = 0;
	std::stringstream sstream;

	// iterate through the data string to get the actual values
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i] != ' ')
		{
			// if current char isn't a space, add it to the stringstream
			sstream << data[i];
		}

		if (data[i] == ' ' || i == data.size() - 1)
		{
			// if this character in the string isn't a space, cast the stringstream data to a float
			vec3values[j] = static_cast<float>(atof(sstream.str().c_str()));
			j++;

			// clear the stringstream
			sstream.str("");
		}
	}

	// assign the data values to an actual vec3
	cbmath::vec3 vec = cbmath::vec3(vec3values[0], vec3values[1], vec3values[2]);
	m_mapVec3[name] = vec;
}

// parse a string into a vec4
// written by: Wednesday-David
void SaveManager::parseVec4(std::string name, std::string data)
{
	float vec4values[4] = { 0, 0, 0, 0 };
	int j = 0;
	std::stringstream sstream;

	// iterate through the data string to get the actual values
	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i] != ' ')
		{
			// if current char isn't a space, add it to the stringstream
			sstream << data[i];
		}

		if (data[i] == ' ' || i == data.size() - 1)
		{
			// if this character in the string isn't a space, cast the stringstream data to a float
			vec4values[j] = static_cast<float>(atof(sstream.str().c_str()));
			j++;

			// clear the stringstream
			sstream.str("");
		}
	}

	// assign the data values to an actual vec4
	cbmath::vec4 vec = cbmath::vec4(vec4values[0], vec4values[1], vec4values[2], vec4values[3]);
	m_mapVec4[name] = vec;
}




// format the data from the current data file into a Particle::Data struct
// written by: Ty and Wednesday-David
Particle::Data SaveManager::prepareData(int dataFileSelected)
{
	if (m_dataFileSelected != dataFileSelected)
	{
		loadData(dataFileSelected);
	}

    float lifespanValue, lifespanDelta;
    float massValue, massDelta;
	float restitutionValue, restitutionDelta;
    cbmath::vec3 velocityValue, velocityDelta;
    cbmath::vec4 colorStart, colorEnd;
    int easeType;
    
    lifespanValue		= m_mapFloat["lifespanValue"];
    lifespanDelta		= m_mapFloat["lifespanDelta"];
    massValue			= m_mapFloat["massValue"];
    massDelta			= m_mapFloat["massDelta"];
	restitutionValue	= m_mapFloat["restitutionValue"];
	restitutionDelta	= m_mapFloat["restitutionDelta"];
    velocityValue		= m_mapVec3["velocityValue"];
    velocityDelta		= m_mapVec3["velocityDelta"];
    colorStart			= m_mapVec4["colorStart"];
    colorEnd			= m_mapVec4["colorEnd"];
    easeType			= m_mapInt["easeValue"];
    
    
    Particle::Data particle;
    
    particle.lifespan = Particle::DeltaType<float>{ lifespanValue, lifespanDelta, false };
    particle.mass = Particle::DeltaType<float>{ massValue, massDelta, false };
	particle.restitution = Particle::DeltaType<float>{ restitutionValue, restitutionDelta, false };
    
    particle.velocity = Particle::DeltaType<cbmath::vec3>{ velocityValue, velocityDelta, true };
    
    particle.color = Particle::LifetimeType<cbmath::vec4>{ colorStart, colorEnd, TimingFunctions::getEase(easeType) };

    
    
    return particle;
}




//-----------------------------------------------------------------------------
// saving data

// write the data from the maps to the specified data file
// written by: Ty and Wednesday-David
void SaveManager::saveData(int dataFileSelected)
{
	m_dataFileSelected = dataFileSelected;

	std::ofstream ofs;

	// make sure the specified file number is valid
	if (dataFileSelected > NUM_SAVE_FILES)
	{
		return;
	}

	std::stringstream filePath;
	filePath << m_dataFilePath << "data" << m_dataFileSelected << ".txt";

	ofs.open(filePath.str());
	if (ofs.is_open())
	{
		writeData(ofs, DataType::Vec3);
		writeData(ofs, DataType::Vec4);
		writeData(ofs, DataType::Float);
		writeData(ofs, DataType::Int);
		writeData(ofs, DataType::Bool);
		writeData(ofs, DataType::Char);
	}
	ofs.close();
}




//-----------------------------------------------------------------------------
// getData template functions

// get the value of a variable from the corresponding map
// written by: Wednesday-David
template <typename T>
T SaveManager::getData(std::string name)
{
	T test;
	return test;
}

template <>
cbmath::vec3 SaveManager::getData<cbmath::vec3>(std::string name)
{
	return m_mapVec3[name];
}

template <>
cbmath::vec4 SaveManager::getData<cbmath::vec4>(std::string name)
{
	return m_mapVec4[name];
}

template <>
float SaveManager::getData<float>(std::string name)
{
	return m_mapFloat[name];
}

template <>
int SaveManager::getData<int>(std::string name)
{
	return m_mapInt[name];
}

template <>
bool SaveManager::getData<bool>(std::string name)
{
	return m_mapBool[name];
}

template <>
char SaveManager::getData<char>(std::string name)
{
	return m_mapChar[name];
}




//-----------------------------------------------------------------------------
// setData template functions

// set the value of a variable in the corresponding map
// written by: Wednesday-David
template <typename T>
void SaveManager::setData(std::string name, T data)
{
}

template <>
void SaveManager::setData<cbmath::vec3>(std::string name, cbmath::vec3 data)
{
	m_mapVec3[name] = data;
}

template <>
void SaveManager::setData<cbmath::vec4>(std::string name, cbmath::vec4 data)
{
	m_mapVec4[name] = data;
}

template <>
void SaveManager::setData<float>(std::string name, float data)
{
	m_mapFloat[name] = data;
}

template <>
void SaveManager::setData<int>(std::string name, int data)
{
	m_mapInt[name] = data;
}

template <>
void SaveManager::setData<bool>(std::string name, bool data)
{
	m_mapBool[name] = data;
}

template <>
void SaveManager::setData<char>(std::string name, char data)
{
	m_mapChar[name] = data;
}




//-----------------------------------------------------------------------------
// writeData function

// write all of the values of a certain map to the current data file
// written by: Wednesday-David
void SaveManager::writeData(std::ofstream& ofs, DataType type)
{
	if (type == DataType::Vec3)
	{
		for (std::map<std::string, cbmath::vec3>::iterator iter = m_mapVec3.begin(); iter != m_mapVec3.end(); ++iter)
		{
			ofs << "vec3" << std::endl;
			ofs << iter->first << std::endl;

			ofs << iter->second.x << " "
				<< iter->second.y << " "
				<< iter->second.z << std::endl;

			ofs << std::endl;
		}
	}
	else if (type == DataType::Vec4)
	{
		for (std::map<std::string, cbmath::vec4>::iterator iter = m_mapVec4.begin(); iter != m_mapVec4.end(); ++iter)
		{
			ofs << "vec4" << std::endl;
			ofs << iter->first << std::endl;

			ofs << iter->second.x << " "
				<< iter->second.y << " "
				<< iter->second.z << " "
				<< iter->second.w << std::endl;

			ofs << std::endl;
		}
	}
	else if (type == DataType::Float)
	{
		for (std::map<std::string, float>::iterator iter = m_mapFloat.begin(); iter != m_mapFloat.end(); ++iter)
		{
			ofs << "float" << std::endl;
			ofs << iter->first << std::endl;
			ofs << iter->second << std::endl;

			ofs << std::endl;
		}
	}
	else if (type == DataType::Int)
	{
		for (std::map<std::string, int>::iterator iter = m_mapInt.begin(); iter != m_mapInt.end(); ++iter)
		{
			ofs << "int" << std::endl;
			ofs << iter->first << std::endl;
			ofs << iter->second << std::endl;

			ofs << std::endl;
		}
	}
	else if (type == DataType::Bool)
	{
		for (std::map<std::string, bool>::iterator iter = m_mapBool.begin(); iter != m_mapBool.end(); ++iter)
		{
			ofs << "bool" << std::endl;
			ofs << iter->first << std::endl;
			ofs << iter->second << std::endl;

			ofs << std::endl;
		}
	}
	else if (type == DataType::Char)
	{
		for (std::map<std::string, char>::iterator iter = m_mapChar.begin(); iter != m_mapChar.end(); ++iter)
		{
			ofs << "char" << std::endl;
			ofs << iter->first << std::endl;
			ofs << iter->second << std::endl;

			ofs << std::endl;
		}
	}
}
