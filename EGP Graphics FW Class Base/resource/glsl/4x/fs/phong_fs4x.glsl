/*
	Phong (FS)
	By Dan Buckstein
	Fragment shader that computes Phong shading.
	
	Modified by: ______________________________________________________________
*/

// version
#version 410


// ****
// varyings

layout (location = 0) out vec4 fragColor;

in Pass
{
    vec4 color;
    //vec2 uv;
    vec4 lightVec;
    vec4 normal;
    vec4 view;
} pass;


// ****
// uniforms


// ****
// target


// shader function
void main()
{
    vec4 normal = normalize(pass.normal);
    vec4 light = normalize(pass.lightVec);
    
    float diffuseMultiplier = dot(normalize(pass.view), light);
    vec4 specular = 2 * dot(light, normal) * normal - light;
    
    float reflection = dot(specular, normalize(pass.view));
    
    //diffuse color * diffuse shading + specular color * specular shading + ambient color + ambient shading
    fragColor.rgb = vec3(pass.color * diffuseMultiplier + (specular * reflection));
    fragColor.a = pass.color.a;
    
    
    //get the diffuse multiplier
    
    //pass the calculated color to the renderer
    fragColor = diffuseMultiplier * pass.color;

    //fragColor *= texture(tex_dm, passUV);
}
