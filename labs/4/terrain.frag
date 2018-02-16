#version 150
 
out vec4 outColor;

in vec2 texCoord;
in vec3 normal;
in vec3 vertex;
in vec3 position;

uniform mat4 modelView;
uniform vec3 cameraPosition;
uniform sampler2D tex;

void main(void)
{
	const vec3 lightPosition[3] = vec3[3](vec3(0, 2, -10), vec3(0, 2, 15), vec3(10, 2, 0));
	const vec3 lightColor[3] = vec3[3](vec3(1, 0, 0), vec3(0.9, 0.9, 0.9), vec3(0, 0, 1) );
	const int N = 3;
	const float k_a = 0.8;
	const float k_d = 0.8;
	const float k_s = 1.0;
	const float a = 25;
	vec3 ambient = vec3(0, 0, 0);
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);
	
	vec3 n = normalize(normal);
	for (int i = 0; i < N; ++i)
	{
		vec3 v = normalize(vertex - cameraPosition);
		vec3 s = normalize(lightPosition[i]);
		// Ambient: i_amb = k_a * i_a
		ambient += s * lightColor[i];
		// Diffuse: i_diff = k_d * i_s * cos(theta)
		float cos_theta = max(dot(s, n), 0);
		diffuse += lightColor[i] * cos_theta;
		// Specular: i_spec = k_spec * i_s *cos(theta_s)^a
		// 			 where cos(theta_s) = r_s * v
		//				   r_s = 2*n*(s*n) - s
		vec3 r_s = 2 * n * dot(s, n) - s;
		float cos_theta_s = max(dot(r_s, v), 0);
		specular += lightColor[i] * pow(cos_theta_s, a);
	}
	if (position.y == 0)
	{
		outColor = (k_a * vec4(ambient, 0) + k_d * vec4(diffuse, 1)) * vec4(0, 0, 1, 1) + k_s * vec4(specular, 1.0);
	}
	else
	{
		outColor = (k_a * vec4(ambient, 1.0) + k_d * vec4(diffuse, 1.0)) * texture(tex, texCoord) + k_s * vec4(specular, 1.0);
	}
}
