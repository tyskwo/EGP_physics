//  David Hartman 0923439
//  Ty Wood       0901793
//
//  EGP 425.01
//  Simple Particle Engine
//  17.02.06
//
//  We certify that this work is entirely our own.
//  The assessor of this project may reproduce this project and provide copies to other academic staff,
//  and/or communicate a copy of this project to a plagiarism-checking service,
//  which may retain a copy of the project on its database.





#ifndef Shader_h
#define Shader_h





#include <stdio.h>
#include "egpfw/egpfw.h"





// a class that abstracts the logic of preparing, applying, and executing a shader program
// written by: Ty
class Shader
{
    
private:
    
    // an enum that hold common uniforms' locations
    enum GLSLCommonUniformIndex
    {
        unif_mvp,
        
        unif_lightPos,
        unif_eyePos,
        
        unif_dm,
        unif_sm,
        
        unif_color,
        
        
        GLSLCommonUniformCount
    };
    int glslCommonUniforms[GLSLCommonUniformCount] = { -1 };
    
    
    
    // the shader program
    egpProgram m_program;
    
    
    
public:
    
    // constructor given a file path to a vertex and fragment shader
    Shader(const char* vertexPath, const char* fragmentPath);
    
    
    
    // release the shader upon destruction
    // written by: Ty
    ~Shader() { egpReleaseProgram(&m_program); }
    
    
    
    // return program
    // written by: Ty
    inline egpProgram getProgram()         { return m_program; }
    
    // getters for common uniforms' locations
    // written by: Ty
    inline int getMVPUniform()      { return glslCommonUniforms[unif_mvp];      }
    inline int getColorUniform()    { return glslCommonUniforms[unif_color];    }
    inline int getLightPosUniform() { return glslCommonUniforms[unif_lightPos]; }
    inline int getEyePosUniform()   { return glslCommonUniforms[unif_eyePos];   }
};



#endif /* Shader_h */


