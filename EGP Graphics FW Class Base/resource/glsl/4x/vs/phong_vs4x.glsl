/*
	Phong (VS)
	By Dan Buckstein
	Vertex shader that passes data required to perform Phong shading.
	
	Modified by: ______________________________________________________________
*/

// version
#version 410


// ****
// attributes

layout (location = 0) in vec4 position;
layout (location = 2) in vec4 normal;

//layout (location = 8) in vec4 texcoord;



// ****
// uniforms

uniform mat4 mvp;
uniform vec4 lightPos;
uniform vec4 eyePos;

uniform vec4 color;



// ****
// varyings

out Pass
{
    vec4 color;
    //vec2 uv;
    vec4 lightVec;
    vec4 normal;
    vec4 view;
} pass;




// shader function
void main()
{
	// ****
	// set clip position
    gl_Position = mvp * position;

	// ****
	// pass data
    //pass.uv = texcoord.xy;
    pass.color = color;
    
    // light ray
    pass.lightVec = lightPos - position;
    pass.normal = vec4(normal.xyz, 0.0);
    
    
    pass.view = eyePos - position;
}
