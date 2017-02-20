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





#include "SaveManager.h"

#include <iostream>
#include <fstream>
#include <sstream>





SaveManager::SaveManager(std::string dataFilePath)
:m_dataFilePath(dataFilePath)
{}





SaveManager::~SaveManager()
{}




//-----------------------------------------------------------------------------
// data loading and parsing

void SaveManager::loadData()
{
	// read data type
	// read in variable values based on type
	// assign values to a Particle::Data struct

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
				parseVec3(name, data);
			}
			else if (type == "vec4")
			{
				parseVec4(name, data);
			}
			else if (type == "float")
			{
				m_mapFloat.insert(std::pair<std::string, float>(name, static_cast<float>(atof(data.c_str()))));
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
			sstream << data[i];
		}

		if (data[i] == ' ' || i == data.size() - 1)
		{
			// if this character in the string isn't a space, cast the stringstream data to a float
			vec3values[j] = static_cast<float>(atof(sstream.str().c_str()));
			j++;

			sstream.str("");
		}
	}

	// assign the data values to an actual vec3
	cbmath::vec3 vec = cbmath::vec3(vec3values[0], vec3values[1], vec3values[2]);
	m_mapVec3.insert(std::pair<std::string, cbmath::vec3>(name, vec));
}





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
			sstream << data[i];
		}

		if (data[i] == ' ' || i == data.size() - 1)
		{
			// if this character in the string isn't a space, cast the stringstream data to a float
			vec4values[j] = static_cast<float>(atof(sstream.str().c_str()));
			j++;

			sstream.str("");
		}
	}

	// assign the data values to an actual vec4
	cbmath::vec4 vec = cbmath::vec4(vec4values[0], vec4values[1], vec4values[2], vec4values[3]);
	m_mapVec4.insert(std::pair<std::string, cbmath::vec4>(name, vec));
}





Particle::Data SaveManager::prepareData()
{
    float lifespanValue, lifespanDelta;
    float massValue, massDelta;
    cbmath::vec3 velocityValue, velocityDelta;
    cbmath::vec4 colorStart, colorEnd;
    
    lifespanValue = m_mapFloat["lifespanValue"];
    lifespanDelta = m_mapFloat["lifespanDelta"];
    massValue     = m_mapFloat["massValue"];
    massDelta     = m_mapFloat["massDelta"];
    velocityValue = m_mapVec3["velocityValue"];
    velocityDelta = m_mapVec3["velocityDelta"];
    colorStart    = m_mapVec4["colorStart"];
    colorEnd      = m_mapVec4["colorEnd"];
    
    
    Particle::Data particle;
    
    particle.lifespan = Particle::DeltaType<float>{ lifespanValue, lifespanDelta, false };
    particle.mass = Particle::DeltaType<float>{ massValue, massDelta, false };
    
    particle.velocity = Particle::DeltaType<cbmath::vec3>{ velocityValue, velocityDelta, true };
    
    particle.color = Particle::LifetimeType<cbmath::vec4>{ colorStart, colorEnd, TimingFunctions::CircularEaseOut };
    
    
    return particle;
}






void SaveManager::saveData()
{
	std::ofstream ofs;

	ofs.open(m_dataFilePath);
	if (ofs.is_open())
	{
		writeData(ofs, DataType::Vec3);
		writeData(ofs, DataType::Vec4);
		writeData(ofs, DataType::Float);
		writeData(ofs, DataType::Bool);
		writeData(ofs, DataType::Char);
	}
	ofs.close();
}





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
bool SaveManager::getData<bool>(std::string name)
{
	return m_mapBool[name];
}

template <>
char SaveManager::getData<char>(std::string name)
{
	return m_mapChar[name];
}





template <typename T>
void SaveManager::setData(std::string name, T data) {}

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
void SaveManager::setData<bool>(std::string name, bool data)
{
	m_mapBool[name] = data;
}

template <>
void SaveManager::setData<char>(std::string name, char data)
{
	m_mapChar[name] = data;
}





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
