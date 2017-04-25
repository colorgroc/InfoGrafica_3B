#include "Object.h"

Object::Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef)   {
	this->scale = scale;
	this->rotation = rotation;
	this->position = position;
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
		glBindVertexArray(0);
	}
}


Object::~Object() {}

void Object::Draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
void Object::Move(vec3 translation) {

}
void Object::Rotate(vec3 rota) {

}
void Object::Scale(vec3 scal) {

}

mat4 Object::GetModelMatrix() {
	mat4 ONLY_MODEL;
	return ONLY_MODEL;
}

/*vec3 Object::GetPosition() {
	return 0;
}*/

void Object::Delete() {

}