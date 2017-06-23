#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec4 modelColor;

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
	 vec4 ambientLightColor;
	 vec4 diffuseLightColor;
	 vec4 specularLightColor;
	 vec4 lightPos; 
} allLights[MAX_LIGHTS];

layout( location = 0 ) out vec4 color;


void main()
{
	Light light = allLights[0];
	vec3 linearColor = vec3(0);
    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * light.ambientLightColor;

	vec3 normal = normalize(Normal);
	vec3 lightDir;
    lightDir = normalize(-light.lightPos.xyz);
    
	//vec3 lightDir = normalize(lightPos - FragPos); 
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = diff * light.diffuseLightColor;
	
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
	vec4 specular = specularStrength * light.specularLightColor * spec;

	color = (ambient + specular + diffuse) * modelColor;
	//color = specular * modelColor;
};