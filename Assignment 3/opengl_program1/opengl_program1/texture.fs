#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in mat3 TBN;

uniform vec4 lightColor;
uniform vec3 viewPos;
uniform vec4 modelColor;
uniform sampler2D image;
uniform sampler2D normalMap;
uniform bool isNorEnabled;
uniform bool isTexEnabled;

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
	 vec4 ambientLightColor;
	 vec4 diffuseLightColor;
	 vec4 specularLightColor;
	 vec4 lightPos; 
} allLights[MAX_LIGHTS];

layout( location = 0 ) out vec4 color;

vec3 ApplyLight(Light light) {


    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * light.ambientLightColor;
	vec3 normal;

	if(isNorEnabled) {
		normal = texture(normalMap, TexCoord).rgb;
		normal = normalize(normal * 2.0 - 1.0);   
		normal = normalize(TBN * normal);
	}
	else {
		normal = normalize(Normal);
	}

	vec3 lightDir;
	if(light.lightPos.w == 0.0) {
        //directional light
        lightDir = normalize(light.lightPos.xyz);
    } else {
        //point light
        lightDir = normalize(light.lightPos.xyz - FragPos);
    }
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = diff * light.diffuseLightColor;
	
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
	vec4 specular = specularStrength * light.specularLightColor * spec;
    //linear color (color before gamma correction)
    return diffuse + specular;
}


void main()
{
	
	vec3 linearColor = vec3(0);
    for(int i = 0; i < numLights; ++i){
        linearColor += ApplyLight(allLights[i]);
    }

	vec4 finalColor;

	if(isTexEnabled) {
		finalColor = texture(image, TexCoord);
	}
	else {
		finalColor = modelColor;
	}

	color = ((vec4(linearColor, 1.0f) + (lightColor * 0.1f)) * finalColor);
};