//
//  SaveManager.h
//  egpfwDemo
//
//  Created by Wednesday-David Hartman on 2/8/17.
//  Copyright 2017 Dan Buckstein. All rights reserved.
//

#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#define NUM_SAVE_FILES 4

#include <string>
#include <map>
#include <stdlib.h>

#include "cbmath/cbtkMatrix.h"

#ifdef _WIN32
	#include "..\particledomain\Particle.h"
#else
	#include "Particle.h"
#endif


class SaveManager
{
private:
	void parseVec3(std::string name, std::string data);
	void parseVec4(std::string name, std::string data);
	
	std::map<std::string, cbmath::vec3> m_mapVec3;
	std::map<std::string, cbmath::vec4> m_mapVec4;
	std::map<std::string, float> m_mapFloat;
	std::map<std::string, bool> m_mapBool;
	std::map<std::string, char> m_mapChar;

	std::string m_dataFilePath;

	int m_dataFileSelected;

public:
	enum DataType { Invalid = -1, Vec3, Vec4, Float, Bool, Char };

	SaveManager(std::string dataFilePath);
	~SaveManager();

	void loadData(int dataFileSelected);
	void saveData(int dataFileSelected);
    
    Particle::Data prepareData(int dataFileSelected);


	// getData functions
	template <typename T>
	T getData(std::string name);

	// setData functions
	template <typename T>
	void setData(std::string name, T data);

	// writeData function
	void writeData(std::ofstream& outf, DataType type);
};


#endif /* SAVEMANAGER_H */
