#version 150
in vec3 inFragNormal;
in vec3 inFragVertex;

out vec4 outColor;

uniform float specularExponent;
// Four global lights
uniform vec3 lightPosition[4];
uniform vec3 lightColor[4];
uniform bool isDirectional[4];
uniform vec3 cameraPosition;
uniform mat4 transform;

void main(void)
{
	mat3 transform3 = mat3(transform);
	float k_d = 0.5; 	// Reflectivity
	float k_spec = 1.0;	// Specularity
	float a = specularExponent;
	vec3 n = normalize(transform3 * inFragNormal);
	vec3 v = normalize(cameraPosition - transform3 * inFragVertex);
	vec3 ambient = vec3(0, 0, 0);
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);
	for (int i = 0; i < 4; ++i)
	{
    	vec3 s;
		if (isDirectional[i] == true) 
			s = normalize(lightPosition[i]);
		else
			s = normalize(transform3 * inFragVertex - lightPosition[i]);
		// Ambient: i_amb = k_d * i_a
		ambient += k_d * s * lightColor[i];
		// Diffuse: i_diff = k_d * i_s * cos(theta)
		float cos_theta = max(dot(s, n), 0);
		diffuse += k_d * lightColor[i] * cos_theta;
		// Specular: i_spec = k_spec * i_s *cos(theta_s)^a
		// 			 where cos(theta_s) = r_s * v
		//				   r_s = 2*n*(s*n) - 
		vec3 r_s = normalize(2 * n * dot(s, n) - s);
		float cos_theta_s = max(dot(r_s, v), 0);
		specular += k_spec * lightColor[i] * pow(cos_theta_s, a);
	}
	
	outColor = vec4(ambient, 1.0) + vec4(diffuse, 0.0) + vec4(specular, 0.0);
}