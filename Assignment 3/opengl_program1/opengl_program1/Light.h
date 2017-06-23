#ifndef LIGHT_H
#define LIGHT_H

struct Light {
	glm::vec4 position;
	glm::vec4 ambientLightColor;
	glm::vec4 diffuseLightColor;
	glm::vec4 specularLightColor;
};


typedef enum { NONE, X, Y, Z } ROTATE_AROUND;
ROTATE_AROUND m_lightRotateAxes = NONE;

float ptAmbientLightColor[4] = { 1.0,1.0,1.0,1.0 };
float ptDiffuseLightColor[4] = { 1.0,1.0,1.0,1.0 };
float ptSpecularLightColor[4] = { 1.0,1.0,1.0,1.0 };

float dirAmbientLightColor[4] = { 1.0,1.0,1.0,1.0 };
float dirDiffuseLightColor[4] = { 1.0,1.0,1.0,1.0 };
float dirSpecularLightColor[4] = { 1.0,1.0,1.0,1.0 };

std::vector<Light> lights;
glm::vec3 ptLtPos;
glm::vec3 dirLtDir;


void initLights() {

	Light ptLight;
	//dir
	ptLight.position = glm::vec4(camPosition, 1.0f);
	//ptLight.ambientLightColor = glm::vec4(ptAmbientLightColor[0], ptAmbientLightColor[1], ptAmbientLightColor[2], ptAmbientLightColor[3]);
	ptLight.diffuseLightColor = glm::vec4(ptDiffuseLightColor[0], ptDiffuseLightColor[1], ptDiffuseLightColor[2], ptDiffuseLightColor[3]);
	ptLight.specularLightColor = glm::vec4(ptSpecularLightColor[0], ptSpecularLightColor[1], ptSpecularLightColor[2], ptSpecularLightColor[3]);
	lights.push_back(ptLight);
	ptLtPos = ptLight.position;


	Light dirLight;
	//dir
	dirLight.position = glm::vec4(glm::vec3(0, 1, 1),0);
	//dirLight.ambientLightColor = glm::vec4(dirAmbientLightColor[0], dirAmbientLightColor[1], dirAmbientLightColor[2], dirAmbientLightColor[3]);
	dirLight.diffuseLightColor = glm::vec4(dirDiffuseLightColor[0], dirDiffuseLightColor[1], dirDiffuseLightColor[2], dirDiffuseLightColor[3]);
	dirLight.specularLightColor = glm::vec4(dirSpecularLightColor[0], dirSpecularLightColor[1], dirSpecularLightColor[2], dirSpecularLightColor[3]);
	lights.push_back(dirLight);
	dirLtDir = dirLight.position;
}

void updateLights() {

	Light ptLight = lights[0];
	//dir
	ptLight.position = glm::vec4(lights[0].position.x, lights[0].position.y, lights[0].position.z, 1.0f);
	//ptLight.ambientLightColor = glm::vec4(ptAmbientLightColor[0], ptAmbientLightColor[1], ptAmbientLightColor[2], ptAmbientLightColor[3]);
	ptLight.diffuseLightColor = glm::vec4(ptDiffuseLightColor[0], ptDiffuseLightColor[1], ptDiffuseLightColor[2], ptDiffuseLightColor[3]);
	ptLight.specularLightColor = glm::vec4(ptSpecularLightColor[0], ptSpecularLightColor[1], ptSpecularLightColor[2], ptSpecularLightColor[3]);
	lights[0] = ptLight;

	Light dirLight = lights[1];
	//dir
	//dirLight.position = glm::vec4(camPosition, 1.0f);
	//dirLight.ambientLightColor = glm::vec4(dirAmbientLightColor[0], dirAmbientLightColor[1], dirAmbientLightColor[2], dirAmbientLightColor[3]);
	dirLight.diffuseLightColor = glm::vec4(dirDiffuseLightColor[0], dirDiffuseLightColor[1], dirDiffuseLightColor[2], dirDiffuseLightColor[3]);
	dirLight.specularLightColor = glm::vec4(dirSpecularLightColor[0], dirSpecularLightColor[1], dirSpecularLightColor[2], dirSpecularLightColor[3]);
	lights[1] = dirLight;

}

void rotateLight() {
	//glm::vec3 ptLtPos = lights[0].position;
	if (m_lightRotateAxes == NONE) {
		ptLtPos = ptLtPos;
	}
	else if (m_lightRotateAxes == X) {
		double r = sqrt(pow(ptLtPos.y, 2) + pow(ptLtPos.z, 2));
		double currX = acos(ptLtPos.z/r);
		if (ptLtPos.y <= 0) {
			currX = -currX;
		}
		currX += glm::radians(0.1f);
		ptLtPos.y = r*sin(currX);
		ptLtPos.z = r*cos(currX);
	}
	else if (m_lightRotateAxes == Y) {
		double r = sqrt(pow(ptLtPos.z, 2) + pow(ptLtPos.x, 2));
		double currY = acos(ptLtPos.x / r);
		if (ptLtPos.z <= 0) {
			currY = -currY;
		}
		currY += glm::radians(0.1f);
		ptLtPos.z = r*sin(currY);
		ptLtPos.x = r*cos(currY);
	}
	else if (m_lightRotateAxes == Z) {
		double r = sqrt(pow(ptLtPos.x, 2) + pow(ptLtPos.y, 2));
		double currZ = acos(ptLtPos.y / r);
		if (ptLtPos.x <= 0) {
			currZ = -currZ;
		}
		currZ += glm::radians(0.1f);
		ptLtPos.x = r*sin(currZ);
		ptLtPos.y = r*cos(currZ);
	}
	lights[0].position = glm::vec4(ptLtPos, 1.0f);
}

void dirLightDirChange() {
	lights[1].position = glm::vec4(dirLtDir, 0.0f);
}

#endif // !CAMERA_H