/*
	Pass Texcoord
	By Dan Buckstein
	Vertex shader that passes texcoord attribute down pipeline.
	
	Modified by: ______________________________________________________________
 */

// version
#version 410


// ****
// attributes
layout (location = 0) in vec4 position;
layout (location = 8) in vec4 texcoord;

// ****
// uniforms

uniform mat4 mvp;


// ****
// varyings

out vec2 passUV;


// shader function
void main()
{
    passUV = texcoord.xy;
    
    gl_Position = mvp * position;
}
