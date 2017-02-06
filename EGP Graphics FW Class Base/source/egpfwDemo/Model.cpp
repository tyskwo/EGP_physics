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

#include "Model.h"



Model::Model(Shader* shader, egpVertexArrayObjectDescriptor* vao)
{
    this->m_shader = shader;
    this->m_vao    = vao;
}
