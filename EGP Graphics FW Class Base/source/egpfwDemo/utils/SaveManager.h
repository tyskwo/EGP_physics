//
//  SaveManager.h
//  egpfwDemo
//
//  Created by Wednesday-David Hartman on 2/8/17.
//  Copyright � 2017 Dan Buckstein. All rights reserved.
//

#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "cbmath/cbtkMatrix.h"
#include "Particle.h"

#include <string>
#include <map>
#include <stdlib.h>

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

public:
	enum DataType { Invalid = -1, Vec3, Vec4, Float, Bool, Char };

	SaveManager(std::string dataFilePath);
	~SaveManager();

	void loadData();
	void saveData();
    
    Particle::Data prepareData();


	// getData functions
	template <typename T>
	T getData(std::string name);
};


#endif /* SAVEMANAGER_H */
