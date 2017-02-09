//
//  SaveManager.h
//  egpfwDemo
//
//  Created by Wednesday-David Hartman on 2/8/17.
//  Copyright © 2017 Dan Buckstein. All rights reserved.
//

#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "cbmath\cbtkMatrix.h"

#include <string>
#include <vector>
#include <map>

class SaveManager
{
private:
	struct Data
	{
		std::vector<std::string> m_dataTypes;
	};

	std::map<std::string, cbmath::vec3> m_mapVec3;
	std::map<std::string, cbmath::vec4> m_mapVec4;
	std::map<std::string, float> m_mapFloat;

	std::string m_dataFilePath;

public:
	SaveManager(std::string dataFilePath);
	~SaveManager();

	void loadData();
	void saveData();

	void printData();
};


#endif /* SAVEMANAGER_H */
