/*
	Set Clip Position VS
	By Dan Buckstein
	Vertex shader that fulfills its primary purpose: set GL's clip position.
*/

#version 410

layout (location = 0) in vec4 position;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * position;
}