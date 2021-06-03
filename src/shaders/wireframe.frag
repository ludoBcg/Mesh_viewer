// Fragment shader
#version 150

// UNIFORMS
uniform vec3 u_diffuseColor;

// INPUT	
in vec3 vecN;
in vec3 vecL;

// OUTPUT
out vec4 frag_color;


float diffuse(in vec3 _N, in vec3 _L)
{
	// Calculate the diffuse (Lambertian) reflection term
    return max(0.0, dot(_N, _L));
}


// MAIN
void main()
{
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float diffuse = diffuse(vecN, vecL);
	color.rgb += u_diffuseColor * diffuse;
	
	frag_color = color;
}

