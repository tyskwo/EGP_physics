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



#ifndef Model_h
#define Model_h



#include "egpfw/egpfw.h"
#include "cbmath/cbtkMatrix.h"
#include "Shader.h"



class Model
{
    
private:
    
    Shader* m_shader;
    
    egpVertexArrayObjectDescriptor* m_vao;
    
    
public:
    
    Model(Shader* shader, egpVertexArrayObjectDescriptor* vao);
    ~Model() { delete m_shader; m_shader = NULL; }
    
    inline Shader* getShader() { return m_shader; }
    inline egpVertexArrayObjectDescriptor* getVAO() { return m_vao; }
    
    void renderAt(cbmath::mat4 matrix);
};



#endif /* Model_h */
