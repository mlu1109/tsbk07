#version 150

in vec2 texCoord;
in vec3 normal;
in vec3 vertex;

out vec4 outColor;

uniform sampler2D texUnit;
uniform float specularExponent;
// Four global lights
uniform vec3 lightPosition[4];
uniform vec3 lightColor[4];
uniform bool isDirectional[4];
uniform vec3 cameraPosition;
uniform float k_a;
uniform float k_d;
uniform float k_s;

void main(void)
{
    float a = specularExponent;
    vec3 n = normalize(normal);
    vec3 v = normalize(cameraPosition - vertex);
    vec3 ambient = vec3(0, 0, 0);
    vec3 diffuse = vec3(0, 0, 0);
    vec3 specular = vec3(0, 0, 0);
    for (int i = 0; i < 4; ++i)
    {
        vec3 s;
        if (isDirectional[i] == true) 
            s = normalize(lightPosition[i]);
        else
            s = normalize(vertex - lightPosition[i]);	
        // Ambient: i_amb = k_a * i_a
        ambient += k_a * s * lightColor[i];
        // Diffuse: i_diff = k_d * i_s * cos(theta)
        float cos_theta = max(dot(s, n), 0);
        diffuse += k_d * lightColor[i] * cos_theta;

        // Specular: i_spec = k_s * i_s *cos(theta_s)^a
        // 			 where cos(theta_s) = r_s * v
        //				   r_s = 2*n*(s*n) - s
        vec3 r_s = 2 * n * dot(s, n) - s;
        float cos_theta_s = max(dot(r_s, v), 0);
        specular += k_s * lightColor[i] * pow(cos_theta_s, a);
    }
    
    vec3 color = vec3(0, 0, 0);
    outColor = (vec4(ambient, 1.0) + vec4(diffuse, 0.0)) * texture(texUnit, texCoord) + vec4(specular, 1.0);
}
