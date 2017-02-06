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



#ifndef _GEOMETRY_H
#define _GEOMETRY_H



namespace Octahedron
{
    const int Vertices = 6;
    const int Indices  = 24;
    
    
    
    const float Positions[Vertices * 3] =
    {
         1.0f,  0.0f,  0.0f,
         0.0f,  0.0f, -1.0f,
        -1.0f,  0.0f,  0.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  1.0f,  0.0f,
         0.0f, -1.0f,  0.0f
    };
    const float Colors[Vertices * 3] =
    {
        1.0f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.0f,
        0.0f,  0.5f,  0.5f,
        0.5f,  0.5f,  1.0f,
        0.5f,  1.0f,  0.5f,
        0.5f,  0.0f,  0.5f
    };
    const unsigned int IndexOrder[Indices] =
    {
        0,1,4, // northern hemisphere
        1,2,4,
        2,3,4,
        3,0,4,
        
        0,3,5, // southern hemisphere
        3,2,5,
        2,1,5,
        1,0,5
    };
    
    
    
    const float        *getPositions();
    const float        *getColors();
    const unsigned int *getIndeces();
          unsigned int  getVertexCount();
          unsigned int  getIndexCount();
}

#endif	// _GEOMETRY_H
