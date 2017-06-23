#ifndef LIGHT_H
#define LIGHT_H

struct Light {
	glm::vec4 position;
	glm::vec4 ambientLightColor;
	glm::vec4 diffuseLightColor;
	glm::vec4 specularLightColor;
};

float dirAmbientLightColor[4] = { 1.0,1.0,1.0,1.0 };
float dirDiffuseLightColor[4] = { 1.0,1.0,1.0,1.0 };
float dirSpecularLightColor[4] = { 1.0,1.0,1.0,1.0 };

std::vector<Light> lights;


void initLights() {

	Light dirLight;
	//dir
	dirLight.position = glm::vec4(camPosition, 1.0f);
	dirLight.ambientLightColor = glm::vec4(dirAmbientLightColor[0], dirDiffuseLightColor[1], dirDiffuseLightColor[2], dirDiffuseLightColor[3]);
	dirLight.diffuseLightColor = glm::vec4(dirDiffuseLightColor[0], dirDiffuseLightColor[1], dirDiffuseLightColor[2], dirDiffuseLightColor[3]);
	dirLight.specularLightColor = glm::vec4(dirSpecularLightColor[0], dirSpecularLightColor[1], dirSpecularLightColor[2], dirSpecularLightColor[3]);
	lights.push_back(dirLight);
}

void updateLights() {

	Light dirLight = lights[0];
	//dir
	//dirLight.position = glm::vec4(camPosition, 1.0f);
	dirLight.ambientLightColor = glm::vec4(dirAmbientLightColor[0], dirDiffuseLightColor[1], dirDiffuseLightColor[2], dirDiffuseLightColor[3]);
	dirLight.diffuseLightColor = glm::vec4(dirDiffuseLightColor[0], dirDiffuseLightColor[1], dirDiffuseLightColor[2], dirDiffuseLightColor[3]);
	dirLight.specularLightColor = glm::vec4(dirSpecularLightColor[0], dirSpecularLightColor[1], dirSpecularLightColor[2], dirSpecularLightColor[3]);
	lights[0] = dirLight;
}


#endif // !CAMERA_H