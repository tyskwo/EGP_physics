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

in pass
{
    vec4 color;
    //vec2 uv;
    vec4 lightVec;
    vec4 normal;
}


// ****
// uniforms


// ****
// target


// shader function
void main()
{
	// ****
	// this example: phong shading algorithm
    
    vec3 normal = normalize(pass.normal);
    
    float lambertian = max(dot(lightVec,normal), 0.0);
    float specular = 0.0;
    
    if(lambertian > 0.0)
    {
        
        vec3 viewDir = normalize(-vertPos);

        
        vec3 reflectDir = reflect(-lightVec, normal);
        specAngle = max(dot(reflectDir, viewDir), 0.0);

        specular = pow(specAngle, shininess/4.0);
    }
    
    
    
    fragColor.rgb = vec3(kd);
    fragColor.a = color.a;

	// ****
	// output: phong

	// ****
	// extra: apply textures as well
    //fragColor *= texture(tex_dm, passUV);
}
