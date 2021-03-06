#ifndef TWEAKBAR_H
#define TWEAKBAR_H

#include <AntTweakBar.h>
#include "Camera.h"
#include "Light.h"


float modelColor[4] = { 1.0, 0.5, 0.0, 1.0 };
float lightColor[4] = { 1.0,1.0,1.0,1.0 };

bool CCW = false;

typedef enum { FILL, LINE, POINT } MESH_TYPE;
MESH_TYPE m_currentMesh;

typedef enum { CUBE, CYBORG, ROCK, TWO_CUBES } MODEL_TYPE;
MODEL_TYPE m_currentModel;

typedef enum BOOL_TYPE : bool { ON = true, OFF = false };
bool zBuffering = OFF;
bool dirLightEnable = OFF;

void callATB(TwBar* bar, GLFWwindow* window);

inline void TwEventMouseButtonGLFW3(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	TwEventMouseButtonGLFW(button, action);
}
inline void TwEventMousePosGLFW3(GLFWwindow* /*window*/, double xpos, double ypos)
{
	TwMouseMotion(int(xpos), int(ypos));
}
inline void TwEventMouseWheelGLFW3(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
	TwEventMouseWheelGLFW(yoffset);
}
inline void TwEventKeyGLFW3(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
	TwEventKeyGLFW(key, action);
}
inline void TwEventCharGLFW3(GLFWwindow* /*window*/, int codepoint)
{
	TwEventCharGLFW(codepoint, GLFW_PRESS);
}
inline void TwWindowSizeGLFW3(GLFWwindow* /*window*/, int width, int height)
{
	TwWindowSize(width, height);
}

void renderOrientation() {
	if (!CCW) {
		glFrontFace(GL_CCW);
	}
	else {
		glFrontFace(GL_CW);
	}
}

void renderModel() {
	if (m_currentMesh == FILL) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (m_currentMesh == LINE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (m_currentMesh == POINT) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
}

void TW_CALL DefCam(void /*clientData*/) {
	camPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 10.0f);
	cameraDirection = glm::normalize(camPosition - cameraTarget);
	cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
}

void TW_CALL DefLight(void /*clientData*/) {
	lights[0].position = glm::vec4(glm::vec3(0.0f, 0.0f, 5.0f), 1.0f);
}

void bufferMode() {
	if (zBuffering) {
		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	else {
		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
	}
}


void callATB(TwBar *bar, GLFWwindow *window) {

	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	TwWindowSize(1024, 768);

	// Create a tweak bar
	bar = TwNewBar("TweakBar");

	//// Add 'wire' to 'bar': it is a modifable variable of type TW_TYPE_BOOL32 (32 bits boolean). Its key shortcut is [w].
	//TwAddVarRW(bar, "wire", TW_TYPE_BOOL32, &wire,
	//	" label='Wireframe mode' key=w help='Toggle wireframe display mode.' ");

	TwEnumVal Models[] = { { CUBE, "Cube" },{ CYBORG, "Cyborg" },{ ROCK, "Rock" },{ TWO_CUBES, "Two Cubes" } };
	TwType ModelTwType = TwDefineEnum("ModelType", Models, 4);
	TwAddVarRW(bar, "Model", ModelTwType, &m_currentModel, NULL);

	TwAddVarRW(bar, "nearView", TW_TYPE_FLOAT, &nearPlane, " label='Near plane distance' ");

	TwAddVarRW(bar, "farView", TW_TYPE_FLOAT, &farPlane, " label='Far plane distance' ");

	TwEnumVal BoolCCW[] = { { ON, "On" },{ OFF, "Off" } };
	TwType BoolCCWTwType = TwDefineEnum("BoolBuffType", BoolCCW, 2);
	TwAddVarRW(bar, "wire", BoolCCWTwType, &CCW, NULL);
	//TwAddVarRW(bar, "wire", TW_TYPE_BOOL32, &CCW, " label='CCW orientation?' ");

	TwAddVarRW(bar, "modelColor", TW_TYPE_COLOR3F, &modelColor, " label='Model color' ");

	TwAddVarRW(bar, "dirLightEnable", TW_TYPE_BOOL32, &dirLightEnable, " label='dirLightEnable' ");
	TwAddVarRW(bar, "dirAmbientLightColor", TW_TYPE_COLOR3F, &dirAmbientLightColor, " label='Dir Ambient Light color' ");
	TwAddVarRW(bar, "dirDiffuseLightColor", TW_TYPE_COLOR3F, &dirDiffuseLightColor, " label='Dir Diffuse light color' ");
	TwAddVarRW(bar, "dirSpecularLightColor", TW_TYPE_COLOR3F, &dirSpecularLightColor, " label='Dir Specular light color' ");

	TwEnumVal BoolBuff[] = { { ON, "On" },{ OFF, "Off" } };
	TwType BoolBuffTwType = TwDefineEnum("BoolBuffType", BoolBuff, 2);
	TwAddVarRW(bar, "zBuffering", BoolBuffTwType, &zBuffering, NULL);

	TwEnumVal Meshes[] = { { FILL, "Fill" },{ LINE, "Line" },{ POINT, "Point" } };
	TwType MeshTwType = TwDefineEnum("MeshType", Meshes, 3);
	TwAddVarRW(bar, "Mesh", MeshTwType, &m_currentMesh, NULL);

	TwAddButton(bar, "resetCam", (TwButtonCallback)DefCam, NULL, " label='Reset Camera' ");

	// Set GLFW event callbacks
	// - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW3);
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW3);
	glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW3);
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW3);

}

#endif TWEAKBAR_H