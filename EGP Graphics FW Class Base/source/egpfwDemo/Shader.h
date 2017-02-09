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



class Shader
{
    
private:
    enum GLSLCommonUniformIndex
    {
        unif_mvp,
        
        unif_lightPos,
        unif_eyePos,
        
        unif_dm,
        unif_sm,
        
        unif_color,
        
        //-----------------------------
        GLSLCommonUniformCount
    };
    int glslCommonUniforms[GLSLCommonUniformCount] = { -1 };
    
    egpProgram m_program;
    
    
    
public:
    
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader() { egpReleaseProgram(&m_program); }
    
    inline egpProgram getProgram()         { return m_program; }
    inline int        getMVPUniform()      { return glslCommonUniforms[unif_mvp];      }
    inline int        getColorUniform()    { return glslCommonUniforms[unif_color];    }
    inline int        getLightPosUniform() { return glslCommonUniforms[unif_lightPos]; }
    inline int        getEyePosUniform()   { return glslCommonUniforms[unif_eyePos];   }
};



#endif /* Shader_h */


