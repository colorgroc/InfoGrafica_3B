#include "Camera.h"
Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov) :
	cameraPos(position), Sensivility(sensitivity), FOV(fov) {
	
	DeltaTime = 0.0f;
	Lastframe = 0.0f;
	YAW = -90;
	PITCH = 0;
	firstMouse = true;
	LastMy = 400;
	LastMx = 400;

	//cameraFront = vec3(0.0, 0.0, -1.0);
	cameraFront = normalize(cameraPos - direction);
	cameraX = cross(cameraFront,vec3(0.0, 1.0, 0.0));
	cameraUp = cross(cameraX, cameraFront);
}
void Camera::DoMovement(GLFWwindow *window) {
	GLfloat currTime = glfwGetTime();
	DeltaTime = currTime - Lastframe;
	Lastframe = currTime;
	GLfloat speed = 1.f * DeltaTime;

	GLint a = glfwGetKey(window, GLFW_KEY_A);
	GLint w = glfwGetKey(window, GLFW_KEY_W);
	GLint s = glfwGetKey(window, GLFW_KEY_S);
	GLint d = glfwGetKey(window, GLFW_KEY_D);
	GLint q = glfwGetKey(window, GLFW_KEY_Q);
	GLint e = glfwGetKey(window, GLFW_KEY_E);
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
	if (q == 1) {
		cameraPos -= speed * cameraUp;
	}
	if (e == 1) {
		cameraPos += speed * cameraUp;
	}
}

void Camera::MouseMove(GLFWwindow *window, double xpos, double ypos) {
 	if (this->firstMouse) //GIRO DE LA CAMARA
	{
		LastMx = xpos;
		LastMy = ypos;
		firstMouse = false;
	}


	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos; // Reversed since y-coordinates range from bottom to top
	LastMx = xpos;
	LastMy = ypos;

	//GLfloat sensitivity = 0.05f;
	xoffset *= Sensivility;
	yoffset *= Sensivility;

	YAW += xoffset;
	PITCH += yoffset;
	if (PITCH > 89.0f)
		PITCH = 89.0f;
	if (PITCH < -89.0f)
		PITCH = -89.f;

	glm::vec3 front;
	front.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
	front.y = sin(glm::radians(PITCH));
	front.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
	cameraFront = glm::normalize(front);
	
}
void Camera::MouseScroll(GLFWwindow *window, double xScroll, double yScroll) {
	if (FOV >= 1.0f && this->FOV <= 45.0f)
		this->FOV -= yScroll;
	if (this->FOV <= 1.0f)
		this->FOV = 1.0f;
	if (this->FOV >= 45.0f)
		this->FOV = 45.0f;
}
mat4 Camera::LookAt() {
	/*mat4 primera;
	primera[0][0] = cameraX.x;
	primera[0][1] = cameraX.y;
	primera[0][2] = cameraX.z;

	primera[1][0] = cameraUp.x;
	primera[1][1] = cameraUp.y;
	primera[1][2] = cameraUp.z;

	primera[2][0] = cameraFront.x;
	primera[2][1] = cameraFront.y;
	primera[2][2] = cameraFront.z;


	mat4 segunda;
	segunda[0][3] = -cameraPos.x;
	segunda[1][3] = -cameraPos.y;
	segunda[2][3] = -cameraPos.z;

	mat4 total = primera * segunda;*/
	//glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);

	return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}
GLfloat Camera::GetFOV() {
	return FOV;
}
vec3 Camera::posicionCamara()
{
	return this->cameraPos;
}


