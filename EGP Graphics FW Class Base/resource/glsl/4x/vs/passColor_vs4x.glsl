/*
	Pass Color
	By Dan Buckstein
	Vertex shader that passes color attribute down pipeline.
	
	Modified by: ______________________________________________________________
*/

// which version of GLSL is this shader written in
#version 410


// ****
// attributes: data read in directly from a vertex in VBO
// format for a single attribute: 
//		layout (location = <attribute index>) in <type> <name>;

layout (location = 0) in vec4 position;


// ****
// uniforms: values that are the same for the entire primitive
// in GLSL 4.3+ you can hard-set these like attributes: 
//		layout (location = <uniform index>) uniform <type> <name>;
// ...or normally (before 4.3): 
//		uniform <type> <name>;

uniform mat4 mvp;
uniform vec4 color;


// ****
// varyings: data passed to the next stage in the pipeline
// in GLSL 4.x you can use structure format: 
//		out <structure name> {
//			<type> <name>;		// <- do this for each one
//		} <output name>;
// ...or one-by-one (compatible with version 3.x): 
//		out <type> <name>;		// <- do this for each one

out vec4 out_color;


// shader entry point: function executes once per-vertex
void main()
{
    out_color = color;
    
    gl_Position = mvp * position;
}
