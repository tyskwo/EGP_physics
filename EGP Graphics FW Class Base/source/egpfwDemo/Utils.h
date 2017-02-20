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





#ifndef Utils_h
#define Utils_h





#include <stdlib.h>
#include "cbmath/cbtkMatrix.h"




// a namespace for helper functions
namespace Utils
{
    // returns a random number from 0 (inclusive) to a (exclusive)
    // written by: Ty
    template <typename T>
    T randomDelta(T a)
    {
        float random = ((float) rand()) / (float) RAND_MAX;
        T r = random * a;
        
        return a + r;
    }

    
    
    
    // returns a random number from -a (exclusive) to a (exclusive)
    // written by: Ty
    template <typename T>
    T randomDeltaPosNeg(T a)
    {
        return ((float(rand()) / float(RAND_MAX)) * (2 * a)) - a;
    }

    
    
    
    // specialized override of above function to randomize a cbmath::vec3
    // written by: Ty
    template <>
    cbmath::vec3 randomDeltaPosNeg<cbmath::vec3>(cbmath::vec3 a);
}






#endif /* Utils_h */
