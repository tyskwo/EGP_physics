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





#include <stdio.h>
#include "Utils.h"




// returns a random cbmath::vec3 with values from -a (exclusive) to a (exclusive)
// written by: Ty
template <>
cbmath::vec3 Utils::randomDeltaPosNeg<cbmath::vec3>(cbmath::vec3 a)
{
    return cbmath::vec3(randomDeltaPosNeg(a.x),
                        randomDeltaPosNeg(a.y),
                        randomDeltaPosNeg(a.z));
}
