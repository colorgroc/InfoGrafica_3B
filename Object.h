#pragma once
#include <GL\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "Shader.hh"
using namespace glm;


 
class Object {

public:
	GLuint returnVao();
	enum FigureType {
		cube = 0,
	};

	Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef);
	~Object();

 GLuint VBO, VAO, EBO;
	void Draw();
	//void Draw(GLuint textura);
	void Move(vec3 translation);
	void Rotate(vec3 rota);
	void Scale(vec3 scal);
	void Delete();
	mat4 GetModelMatrix();
	vec3 GetPosition();

private:
//	mat4 model;
	vec3 position;
	vec3 scale;
	vec3 rotation;
};
