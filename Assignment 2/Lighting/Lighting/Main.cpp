
#define GLFW_KEY_SPECIAL 255

#include <iostream>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include "TweakBar.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"

int main() {

	TwBar* bar = NULL;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Object load and manipulate", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, 1024, 768);

	Shader lightingShader("lightVertexShader.vs", "lightColorShader.fs");

	GLuint lightModel = glGetUniformLocation(lightingShader.Program, "model");
	GLuint lightView = glGetUniformLocation(lightingShader.Program, "view");
	GLuint lightProjection = glGetUniformLocation(lightingShader.Program, "projection");


	GLuint VertexArrayID;

	std::vector<Vertex> model;
	GLuint vertexbuffer;

	m_currentMesh = FILL;
	m_currentModel = CUBE;
	MODEL_TYPE m_lastModel = CYBORG;
	SHADER_TYPE m_lastShader = FLAT;
	std::string path;

	loadModel(model, m_lastModel);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(Vertex), &model[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::tvec3<float>)));
	glBindVertexArray(0);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Init GUI
	callATB(bar, window);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	initLights();
	Shader shader;

	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

		bufferMode();
		renderModel();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (m_currentModel != m_lastModel) {
			loadModel(model, m_lastModel);
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(Vertex), &model[0], GL_STATIC_DRAW);

			glGenVertexArrays(1, &VertexArrayID);
			glBindVertexArray(VertexArrayID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::tvec3<float>)));
			glBindVertexArray(0);
		}
		
		if (m_currentShader != m_lastShader) {
			if (m_currentShader == SMOOTH) {
				shader = setShader("smoothVertexShader.vs", "smoothFragmentShader.fs");
				m_lastShader = SMOOTH;
			}
			else {
				shader = setShader("flatVertexShader.vs", "flatFragmentShader.fs");
				m_lastShader = FLAT;
			}
		}

		shader.Use();

		computeMatricesFromInputs(window);
		glm::mat4 ModelMatrix = glm::mat4(1.0);

		glBindVertexArray(VertexArrayID);

		glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glUniform4fv(colorID, 1, modelColor);
		glUniform3f(viewPosLoc, camPosition.x, camPosition.y, camPosition.z);
		glUniform1i(numLights, lights.size());

		//global light
		if (glLightEnable) {
			glUniform4fv(lColor, 1, lightColor);
		}
		else {
			glUniform4fv(lColor, 1, lightColorOff);
		}

		//point light
		if (ptLightEnable) {
			glUniform4f(lightPosLoc0, lights[0].position.x, lights[0].position.y, lights[0].position.z, lights[0].position.w);
			glUniform4f(ambientLightColor0ID, lights[0].ambientLightColor.r, lights[0].ambientLightColor.g, lights[0].ambientLightColor.b, lights[0].ambientLightColor.a);
			glUniform4f(diffuseLightColor0ID, lights[0].diffuseLightColor.r, lights[0].diffuseLightColor.g, lights[0].diffuseLightColor.b, lights[0].diffuseLightColor.a);
			glUniform4f(specularLightColor0ID, lights[0].specularLightColor.r, lights[0].specularLightColor.g, lights[0].specularLightColor.b, lights[0].specularLightColor.a);
		}
		else {
			glUniform4f(ambientLightColor0ID, 0, 0, 0, 0);
			glUniform4f(diffuseLightColor0ID, 0, 0, 0, 0);
			glUniform4f(specularLightColor0ID, 0, 0, 0, 0);
		}
		//directional light
		if (dirLightEnable) {
			glUniform4f(lightPosLoc1, lights[1].position.x, lights[1].position.y, lights[1].position.z, lights[1].position.w);
			glUniform4f(ambientLightColor1ID, lights[1].ambientLightColor.r, lights[1].ambientLightColor.g, lights[1].ambientLightColor.b, lights[1].ambientLightColor.a);
			glUniform4f(diffuseLightColor1ID, lights[1].diffuseLightColor.r, lights[1].diffuseLightColor.g, lights[1].diffuseLightColor.b, lights[1].diffuseLightColor.a);
			glUniform4f(specularLightColor1ID, lights[1].specularLightColor.r, lights[1].specularLightColor.g, lights[1].specularLightColor.b, lights[1].specularLightColor.a);
		}
		else {
			glUniform4f(ambientLightColor1ID, 0, 0, 0, 0);
			glUniform4f(diffuseLightColor1ID, 0, 0, 0, 0);
			glUniform4f(specularLightColor1ID, 0, 0, 0, 0);
		}

		updateLights();

		rotateLight();

		glDrawArrays(GL_TRIANGLES, 0, model.size());
		glBindVertexArray(0);
		

		lightingShader.Use();
		ModelMatrix = glm::mat4();
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(lights[0].position.x, lights[0].position.y, lights[0].position.z));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.2f));
		glUniformMatrix4fv(lightModel, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(lightView, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(lightProjection, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 0);
		glBindVertexArray(0);


		
		renderOrientation();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		TwDraw();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(shader.Program);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	TwTerminate();
	glfwTerminate();

	return 0;

}
