#version 150
 
out vec4 outColor;

in vec3 normal;
in vec3 vertex;

uniform mat4 modelView;
uniform vec3 cameraPosition;

void main(void)
{
	const vec3 lightPosition[3] = vec3[3](vec3(0, 2, -10), vec3(0, 2, 10), vec3(10, 2, 0));
	const vec3 lightColor[3] = vec3[3](vec3(1, 0, 0), vec3(1, 1, 1), vec3(0, 0, 1) );
	const int N = 3;
	const float k_a = 0.8;
	const float k_d = 0.8;
	const float k_s = 1.0;
	const float a = 10;
	vec3 ambient = vec3(0, 0, 0);
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);
	
	vec3 n = normalize(normal);
	for (int i = 0; i < N; ++i)
	{
		vec3 v = normalize(vertex - cameraPosition);
		vec3 s = normalize(lightPosition[i]);
		// Ambient: i_amb = k_a * i_a
		ambient += k_a * s * lightColor[i];
		// Diffuse: i_diff = k_d * i_s * cos(theta)
		float cos_theta = max(dot(s, n), 0);
		diffuse += k_d * lightColor[i] * cos_theta;
		// Specular: i_spec = k_spec * i_s *cos(theta_s)^a
		// 			 where cos(theta_s) = r_s * v
		//				   r_s = 2*n*(s*n) - s
		vec3 r_s = 2 * n * dot(s, n) - s;
		float cos_theta_s = max(dot(r_s, v), 0);
		specular += k_s * lightColor[i] * pow(cos_theta_s, a);
	}	
	outColor = vec4(ambient, 1.0) + vec4(diffuse, 1.0) + vec4(specular, 1.0);
}
