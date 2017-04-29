#pragma once
#include <GL\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "Shader.hh"
using namespace glm;
class Camera
{
public:
	Camera(vec3 position,vec3 direction, GLfloat sensitivity, GLfloat fov);
	void DoMovement(GLFWwindow *window);
	void MouseMove(GLFWwindow *window, double xpos, double ypos);
	void MouseScroll(GLFWwindow *window, double xScroll, double yScroll);
	mat4 LookAt();
	GLfloat GetFOV();
	vec3 posicionCamara();
	

private:
	vec3 cameraFront;
	vec3 cameraPos;
	vec3 cameraUp;
	vec3 cameraX;
	GLfloat DeltaTime;
	GLfloat Lastframe;
	GLfloat LastMx;
	GLfloat LastMy;
	GLfloat Sensivility;
	GLboolean firstMouse;
	GLfloat PITCH;
	GLfloat YAW;
	GLfloat FOV;
};
