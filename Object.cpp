#include "Object.h"

Object::Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef) : scale(scale), rotation(rotation), position(position)  {
	if (typef == cube) {
		GLfloat VertexBufferObject[] = {
			//front
			1.0f ,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
			1.0f , -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
			1.0f ,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
			//back
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			1.0f , -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			1.0f ,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,

			1.0f ,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			//left
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
			//right
			1.0f , -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
			1.0f ,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
			1.0f ,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

			1.0f ,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
			1.0f , -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
			1.0f , -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
			//down
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
			1.0f , -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
			1.0f , -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
			1.0f , -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
			//up
			1.0f ,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			1.0f ,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			1.0f ,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)3);
			glEnableVertexAttribArray(1);	
		glBindVertexArray(0);
	}
}


Object::~Object() {}

void Object::Draw() {
	//glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}
void Object::Move(vec3 translation) {
	this->position = translation;
}
void Object::Rotate(vec3 rota) {
	this->rotation = rota;
}
void Object::Scale(vec3 scal) {
	this->scale = scal;
}

mat4 Object::GetModelMatrix() {
	glBindVertexArray(VAO);
	mat4 model;
	model = translate(model, position);
	model = rotate(model, radians(rotation.x), vec3(1.0, 0.0, 0.0));
	model = rotate(model, radians(rotation.y), vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, scale);

	return model;
}

vec3 Object::GetPosition() {
	return position;
}

void Object::Delete() {

}
