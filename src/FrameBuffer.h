#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <SOIL.h>
#include <GL\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	GLint loadTexture(const char* path);
	GLuint loadTextureAlpha(GLchar* path, GLboolean alpha);
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
	const GLuint WIDTH = 800, HEIGHT = 800;
	GLuint framebuffer;
private: 
	//GLuint framebuffer;
	GLuint rbo;
	GLuint textureColorbuffer;
};


