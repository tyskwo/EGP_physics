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





#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H





#include <string>
#include <map>
#include <stdlib.h>

#include "cbmath/cbtkMatrix.h"

#ifdef _WIN32
	#include "..\particledomain\Particle.h"
#else
	#include "Particle.h"
#endif




// a class that handles taking Particle::Data in and out of a text file
// written by: David
class SaveManager
{
    
private:
    
    // functions to parse a cbmath::vec3 and cbmath::vec4, given a parameter name and string of data
	void parseVec3(std::string name, std::string data);
	void parseVec4(std::string name, std::string data);
	
    
    
    // maps to hold each type of parameter data, stored as key-value pairs
	std::map<std::string, cbmath::vec3> m_mapVec3;
	std::map<std::string, cbmath::vec4> m_mapVec4;
	std::map<std::string, float> m_mapFloat;
	std::map<std::string, bool> m_mapBool;
	std::map<std::string, char> m_mapChar;

    
    
    // string containing the path to the file
	std::string m_dataFilePath;

    
    
public:
    
    // enum holding the different types of parameter data
	enum DataType { Invalid = -1, Vec3, Vec4, Float, Bool, Char };

    
    
	SaveManager(std::string dataFilePath);
	~SaveManager();

    
    // functions to load and save the data
	void loadData();
	void saveData();
    
    
    
    // function to prepare the Particle::Data to be loaded into the game
    Particle::Data prepareData();



    // get data given a key
    template <typename T>
	T getData(std::string name);

    

    // set data given a key and the value
    template <typename T>
	void setData(std::string name, T data);


    
    // write data to the text file given an output stream and the type of data
    void writeData(std::ofstream& outf, DataType type);
};





#endif /* SAVEMANAGER_H */
