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





#include "Shader.h"





// constructor given a file path to a vertex and fragment shader
// written by: Ty
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // declare file and shaders
    egpFileInfo files[2];
    egpShader   shaders[2];
    
    
    // array of common uniform names
    const char *commonUniformName[] =
    {
        (const char *)("mvp"),
        (const char *)("lightPos"),
        (const char *)("eyePos"),
        (const char *)("tex_dm"),
        (const char *)("tex_sm"),
        (const char *)("color"),
    };
    
    
    // load files
    files[0] = egpLoadFileContents(vertexPath);
    files[1] = egpLoadFileContents(fragmentPath);
    
    
    // create shaders
    shaders[0] = egpCreateShaderFromSource(EGP_SHADER_VERTEX,   files[0].contents);
    shaders[1] = egpCreateShaderFromSource(EGP_SHADER_FRAGMENT, files[1].contents);
    
    
    // create, link and validate program
    this->m_program = egpCreateProgram();
    egpAttachShaderToProgram(&this->m_program, shaders + 0);
    egpAttachShaderToProgram(&this->m_program, shaders + 1);
    egpLinkProgram(&this->m_program);
    egpValidateProgram(&this->m_program);
    
    
    // release shaders and files
    egpReleaseShader(shaders + 0);
    egpReleaseShader(shaders + 1);
    egpReleaseFileContents(files + 0);
    egpReleaseFileContents(files + 1);
    
    
    // bind uniforms
    egpActivateProgram(&this->m_program);
    for (int u = 0; u < GLSLCommonUniformCount; ++u) glslCommonUniforms[u] = egpGetUniformLocation(&this->m_program, commonUniformName[u]);
    
    
    // disable all
    egpActivateProgram(0);
    egpActivateVAO(0);
}
