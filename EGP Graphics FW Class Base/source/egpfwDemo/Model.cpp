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

#include <stdlib.h>


Model::Model(Shader* shader, egpVertexArrayObjectDescriptor* vao)
{
    this->m_shader = shader;
    this->m_vao    = vao;
}

void Model::renderAt(cbmath::mat4 matrix, cbmath::vec4 cameraPos, cbmath::vec4 color)
{
    egpProgram temp = m_shader->getProgram();
    egpActivateProgram(&temp);
    
    egpSendUniformFloatMatrix(m_shader->getMVPUniform(), UNIF_MAT4, 1, 0, matrix.m);

    egpSendUniformFloat(m_shader->getColorUniform(), UNIF_VEC4, 1, color.v);
    egpSendUniformFloat(m_shader->getLightPosUniform(), UNIF_VEC4, 1, cbmath::vec4(5.0f,5.0f,5.0f,1.0f).v);
    egpSendUniformFloat(m_shader->getEyePosUniform(), UNIF_VEC4, 1, cameraPos.v);
    
    egpActivateVAO(m_vao);
    egpDrawActiveVAO();
}

