#define GLFW_KEY_SPECIAL 255

#include <iostream>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <vector>

// Other includes
#include "Shader.h".0
#include "Camera.h"
#include "TweakBar.h"
#include "Model.h"
#include "Light.h"

// The MAIN function, from here we start the application and run the game loop
int main() {

	TwBar* bar = NULL;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Assignment", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
		
		shader = setShader("smoothVertexShader.vs", "smoothFragmentShader.fs");

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

		
		//directional light
		if (dirLightEnable) {
			glUniform4f(lightPosLoc1, lights[0].position.x, lights[0].position.y, lights[0].position.z, lights[0].position.w);
			glUniform4f(ambientLightColor1ID, lights[0].ambientLightColor.r, lights[0].ambientLightColor.g, lights[0].ambientLightColor.b, lights[0].ambientLightColor.a);
			glUniform4f(diffuseLightColor1ID, lights[0].diffuseLightColor.r, lights[0].diffuseLightColor.g, lights[0].diffuseLightColor.b, lights[0].diffuseLightColor.a);
			glUniform4f(specularLightColor1ID, lights[0].specularLightColor.r, lights[0].specularLightColor.g, lights[0].specularLightColor.b, lights[0].specularLightColor.a);
		}
		else {
			glUniform4f(ambientLightColor1ID, 0, 0, 0, 0);
			glUniform4f(diffuseLightColor1ID, 0, 0, 0, 0);
			glUniform4f(specularLightColor1ID, 0, 0, 0, 0);
		}

		updateLights();

		glDrawArrays(GL_TRIANGLES, 0, model.size());
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