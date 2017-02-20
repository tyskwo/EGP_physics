//  Wednesday-David Hartman 0923439
//  Ty Wood					0901793
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





// class that encapsulates a 3d model and its associated shader program
// written by: Ty
class Model
{
    
private:
    
    // reference to the shader to apply to this model
    Shader* m_shader;
    
    
    
    // reference to the vao that describes this model
    egpVertexArrayObjectDescriptor* m_vao;
    
    
    
public:
    
    // constructor, passing in a shader and vao
    Model(Shader* shader, egpVertexArrayObjectDescriptor* vao);
    
    
    
    //destructor, delete shader on destruction
    //written by: Ty
    ~Model() { delete m_shader; m_shader = NULL; }
    
    
    
    // getters, return the shader and VAO
    // written by: Ty
    inline Shader* getShader() { return m_shader; }
    inline egpVertexArrayObjectDescriptor* getVAO() { return m_vao; }
    
    
    
    // called when this model needs to be rendered, passed in a MVP matric, the camera position, and the color to be rendered as
    void renderAt(cbmath::mat4 matrix,cbmath::vec4 cameraPos, cbmath::vec4 color);
};



#endif /* Model_h */
