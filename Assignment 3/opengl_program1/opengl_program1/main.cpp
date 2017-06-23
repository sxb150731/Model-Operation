
#define GLFW_KEY_SPECIAL 255

#include <iostream>
#include <string>
#define GLEW_STATIC
#include <GL\glew.h>
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

	//nano gui
	/*screen = new nanogui::Screen();
	screen->initialize(window, true);*/

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
	GLuint vertexbuffer[3];
	glGenBuffers(3, vertexbuffer);

	m_currentMesh = FILL;
	m_currentModel = CUBE;
	MODEL_TYPE m_lastModel = TWO_CUBES;
	SHADER_TYPE m_lastShader = FLAT;
	std::string path;

	//Init GUI
	callATB(bar, window);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	initLights();
	Shader shader;

	// Load and create a texture 
	GLuint tex, nor;

	shader = setShader("texture.vs", "texture.fs");

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;

	//loadModel(model, m_lastModel);

	GLuint tangentbuffer;
	GLuint bitangentbuffer;

	//Nano gui init
	//getGUI(window);

	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {


		shader.Use();

		//glDepthFunc(GL_LESS);
		//bufferMode();
		renderModel();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (m_currentModel != m_lastModel) {
			loadModel(model, m_lastModel, tex, nor);
			for (unsigned int i = 0; i<model.size(); i += 3) {

				// Shortcuts for vertices
				glm::vec3 & v0 = model[i + 0].Position;
				glm::vec3 & v1 = model[i + 1].Position;
				glm::vec3 & v2 = model[i + 2].Position;

				// Shortcuts for UVs
				glm::vec2 & uv0 = model[i + 0].TexCoords;
				glm::vec2 & uv1 = model[i + 1].TexCoords;
				glm::vec2 & uv2 = model[i + 2].TexCoords;

				// Edges of the triangle : postion delta
				glm::vec3 deltaPos1 = v1 - v0;
				glm::vec3 deltaPos2 = v2 - v0;

				// UV delta
				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

				// Set the same tangent for all three vertices of the triangle.
				// They will be merged later, in vboindexer.cpp
				tangents.push_back(tangent);
				tangents.push_back(tangent);
				tangents.push_back(tangent);

				// Same thing for binormals
				bitangents.push_back(bitangent);
				bitangents.push_back(bitangent);
				bitangents.push_back(bitangent);

			}


			// See "Going Further"
			for (unsigned int i = 0; i<model.size(); i += 1)
			{
				glm::vec3 & n = model[i].Normal;
				glm::vec3 & t = tangents[i];
				glm::vec3 & b = bitangents[i];

				// Gram-Schmidt orthogonalize
				t = glm::normalize(t - n * glm::dot(n, t));

				// Calculate handedness
				if (glm::dot(glm::cross(n, t), b) < 0.0f) {
					t = t * -1.0f;
				}

			}

			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
			glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(Vertex), &model[0], GL_STATIC_DRAW);

			//glGenBuffers(1, &tangentbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);

			//glGenBuffers(1, &bitangentbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
			glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);

			glGenVertexArrays(1, &VertexArrayID);
			glBindVertexArray(VertexArrayID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::tvec3<float>)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2 * sizeof(glm::tvec3<float>)));
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

			glBindVertexArray(0);

		}

		/*if (m_currentShader != m_lastShader) {
		if (m_currentShader == SMOOTH) {
		shader = setShader("smoothVertexShader.vs", "smoothFragmentShader.fs");
		m_lastShader = SMOOTH;
		}
		else {
		shader = setShader("flatVertexShader.vs", "flatFragmentShader.fs");
		m_lastShader = FLAT;
		}
		}*/


		glUniform1i(glGetUniformLocation(shader.Program, "image"), 0);
		glUniform1i(glGetUniformLocation(shader.Program, "normalMap"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, nor);

		computeMatricesFromInputs(window);
		glm::mat4 ModelMatrix = glm::mat4(1.0);

		glBindVertexArray(VertexArrayID);

		glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glUniform4fv(colorID, 1, modelColor);
		glUniform3f(viewPosLoc, camPosition.x, camPosition.y, camPosition.z);
		glUniform1i(numLights, lights.size());
		glUniform1i(isTexEnabledID, isTexEnabled);
		glUniform1i(isNorEnabledID, isNorEnabled);

		//global light
		/*if (glLightEnable) {
		glUniform4fv(lColor, 1, lightColor);
		}
		else {
		glUniform4fv(lColor, 1, lightColorOff);
		}*/

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
		dirLightDirChange();

		glDrawArrays(GL_TRIANGLES, 0, model.size());
		glBindVertexArray(0);


		/*lightingShader.Use();
		ModelMatrix = glm::mat4();
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(lights[0].position.x, lights[0].position.y, lights[0].position.z));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.2f));
		glUniformMatrix4fv(lightModel, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(lightView, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(lightProjection, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 0);
		glBindVertexArray(0);*/



		renderOrientation();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		TwDraw();

		//Draw nanogui
		//screen->drawContents();
		//screen->drawWidgets();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// Cleanup VBO and shader
	glDeleteBuffers(3, vertexbuffer);
	glDeleteProgram(shader.Program);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	TwTerminate();
	glfwTerminate();

	return 0;

}
