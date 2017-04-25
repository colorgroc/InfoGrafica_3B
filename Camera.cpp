#include "Camera.h"
Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov) :
	cameraPos(position), Sensivility(sensitivity), FOV(fov) {
	
	DeltaTime = 0.0f;
	PITCH = 0.0f;
	YAW = 45.0f;
	LastMx = 400;
	LastMy = 400;
	cameraFront = vec3(0.0, 0.0, -1.0);
	vec3 camDir = normalize(cameraPos - direction);
	cameraX = cross( camDir,vec3(0.0, 1.0, 0.0));
	cameraUp = cross(cameraX, cameraFront);
}
void Camera::DoMovement(GLFWwindow *window) {
	GLfloat currTime = glfwGetTime();
	DeltaTime = currTime - Lastframe;
	Lastframe = currTime;
	GLfloat speed = 3.f * DeltaTime;

	GLint a = glfwGetKey(window, GLFW_KEY_A);
	GLint w = glfwGetKey(window, GLFW_KEY_W);
	GLint s = glfwGetKey(window, GLFW_KEY_S);
	GLint d = glfwGetKey(window, GLFW_KEY_D);
	if (a == 1) {
		cameraPos -= normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}
	if (w == 1) {
		cameraPos += speed * cameraFront;
	}
	if (s == 1) {
		cameraPos -= speed * cameraFront;
	}
	if (d == 1) {
		cameraPos += normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}
}
void Camera::MouseMove(GLFWwindow *window, double xpos, double ypos) {
	if (firstMouse) //GIRO DE LA CAMARA
	{
		LastMx = xpos;
		LastMy = ypos;
		firstMouse = false;
	}


	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos; // Reversed since y-coordinates range from bottom to top
	LastMx = xpos;
	LastMy = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	YAW += xoffset;
	PITCH += yoffset;
	PITCH = clamp(PITCH, -89.f, 89.f);
	YAW = mod(YAW, 360.f);


	glm::vec3 front;
	front.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
	front.y = sin(glm::radians(PITCH));
	front.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
	cameraFront = glm::normalize(front);
}
void Camera::MouseScroll(GLFWwindow *window, double xScroll, double yScroll) {
	if (FOV >= 1.0f && FOV <= 45.0f)
		FOV -= yScroll;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45.0f)
		FOV = 45.0f;
}
mat4 Camera::LookAt() {
	mat4 primera = mat4(cameraX.x, cameraX.y, cameraX.z, 0,
				    cameraUp.x, cameraUp.y, cameraUp.z, 0,
					cameraFront.x, cameraFront.y, cameraFront.z, 0,
					0, 0, 0, 1);

	mat4 segunda = mat4(1, 0, 0, -cameraPos.x,
						0, 1, 0, -cameraPos.y,
						0, 0, 1, -cameraPos.z,
						0, 0, 0, 1);
	mat4 total = primera * segunda;

	return total;
}
GLfloat Camera::GetFOV() {
	return 45.f;
}
