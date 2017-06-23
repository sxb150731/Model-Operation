#ifndef CAMERA_H
#define CAMERA_H

float camInitialFoV = 45.0f;
float camSpeed = 3.0f;

float nearPlane = 0.1f;
float farPlane = 100.0f;

glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraDirection = glm::normalize(camPosition - cameraTarget);
glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

void computeMatricesFromInputs(GLFWwindow* window) {

	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	//===================TRANSLATE CAMERA====================//
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camPosition += cameraDirection * deltaTime * camSpeed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camPosition -= cameraDirection * deltaTime * camSpeed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camPosition -= cameraRight * deltaTime * camSpeed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camPosition += cameraRight * deltaTime * camSpeed;
	}
	// Up
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		camPosition += cameraUp * deltaTime * camSpeed;
	}
	// Down
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		camPosition -= cameraUp * deltaTime * camSpeed;
	}
	//===================ROTATE CAMERA=====================//
	//Rotate around right
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraDirection = glm::rotate(cameraDirection, glm::radians(-0.1f), cameraRight);
		cameraUp = glm::rotate(cameraUp, glm::radians(-0.1f), cameraRight);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraDirection = glm::rotate(cameraDirection, glm::radians(0.1f), cameraRight);
		cameraUp = glm::rotate(cameraUp, glm::radians(0.1f), cameraRight);
	}
	//Rotate around up
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraDirection = glm::rotate(cameraDirection, glm::radians(0.1f), cameraUp);
		cameraRight = glm::rotate(cameraRight, glm::radians(0.1f), cameraUp);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraDirection = glm::rotate(cameraDirection, glm::radians(-0.1f), cameraUp);
		cameraRight = glm::rotate(cameraRight, glm::radians(-0.1f), cameraUp);
	}
	//Rotate around front
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraUp = glm::rotate(cameraUp, glm::radians(0.1f), cameraDirection);
		cameraRight = glm::rotate(cameraRight, glm::radians(0.1f), cameraDirection);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraUp = glm::rotate(cameraUp, glm::radians(-0.1f), cameraDirection);
		cameraRight = glm::rotate(cameraRight, glm::radians(-0.1f), cameraDirection);
	}

	float FoV = camInitialFoV;
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, nearPlane, farPlane);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		camPosition,					// Camera is here
		camPosition + cameraDirection,	// and looks here : at the same position, plus "direction"
		cameraUp						// Head is up (set to 0,-1,0 to look upside-down)
	);
	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


#endif // !CAMERA_H