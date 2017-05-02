#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Shader.hh"
#include "Camera.h"
#include "Object.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
using namespace glm;
using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
Object *objeto;
Object *objeto2; 
Material *material;
Camera *camara;

const GLuint WIDTH = 800, HEIGHT = 800;


glm::vec3 lightPos(0.f, 1.5f, 0.f);

vec3 rotacion;
vec3 mov;

int dir;
int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);

	Shader ambientLight("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	Shader shaderLight("./src/LightVertexShader.vertexshader", "./src/LightFragmentShader.fragmentshader");
	Shader dirLight("./src/dirLight.vertexshader", "./src/dirLight.fragmentshader");
	Shader pointLight("./src/pointLight.vertexshader", "./src/pointLight.fragmentshader");
	Shader focalLight("./src/focalLight.vertexshader", "./src/focalLight.fragmentshader");

	Shader modelShader("./src/modelShader.vertexshader", "./src/modelShader.fragmentshader");
	Model modelo("./src/spider/spider.obj");

	//Shader shaderText("./src/texture.vertexshader", "./src/texture.fragmentshader");
	
	objeto = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(lightPos.x, lightPos.y, lightPos.z), Object::cube);//lampara
	objeto2 = new Object(vec3(0.3f), vec3(0.0, 0.0, 0.0), vec3(0.0f), Object::cube);//cubo grande
	camara = new Camera(vec3(0.0f, 0.0f, 3.0f), vec3(0.0), 0.05, 45.0);
	material = new Material("./src/difuso.png", "./src/especular.png", 50.f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
	
		glfwPollEvents();
		camara->DoMovement(window);

		glClearColor(0.0f, 0.f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//CUBO GRANDE
		//luz ambiental sola
		/*ambientLight.Use();
		vec3 posicion = camara->posicionCamara();
		GLint objectColorLoc = glGetUniformLocation(ambientLight.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(ambientLight.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(ambientLight.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(ambientLight.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); 
		glUniform3f(viewPosLoc, posicion.x,posicion.y, posicion.z);*/
		//luz direccional
		/*dirLight.Use();
		material->ActivateTextures();
		material->SetMaterial(&dirLight);
		material->SetShininess(&dirLight);
		vec3 posicion = camara->posicionCamara();
		glUniform3f(glGetUniformLocation(dirLight.Program, "viewPos"), posicion.x, posicion.y, posicion.z);
		glUniform3f(glGetUniformLocation(dirLight.Program, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(dirLight.Program, "objectColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(dirLight.Program, "lightdir"), 0.0f, -1.0f, 0.0f);*/
		//luz puntual
		pointLight.Use();
		material->ActivateTextures();
		material->SetMaterial(&pointLight);
		material->SetShininess(&pointLight);
		vec3 posicion = camara->posicionCamara();
		GLint objectColorLoc = glGetUniformLocation(pointLight.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(pointLight.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(pointLight.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(pointLight.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(viewPosLoc, posicion.x, posicion.y, posicion.z);
		glUniform1f(glGetUniformLocation(pointLight.Program, "constant"), 1.0f);
		glUniform1f(glGetUniformLocation(pointLight.Program, "linear"), 0.09f);
		glUniform1f(glGetUniformLocation(pointLight.Program, "quadratic"), 0.032f);

	
		//luz focal
		/*focalLight.Use();
		material->ActivateTextures();
		material->SetMaterial(&focalLight);
		material->SetShininess(&focalLight);
		vec3 posicion = camara->posicionCamara();
		GLint objectColorLoc = glGetUniformLocation(focalLight.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(focalLight.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(focalLight.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(focalLight.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(viewPosLoc, posicion.x, posicion.y, posicion.z);
		glUniform1f(glGetUniformLocation(focalLight.Program, "constant"), 1.0f);
		glUniform1f(glGetUniformLocation(focalLight.Program, "linear"), 0.09f);
		glUniform1f(glGetUniformLocation(focalLight.Program, "quadratic"), 0.032f);
		glUniform3f(glGetUniformLocation(focalLight.Program, "focoDir"), 0.0f, -1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(focalLight.Program, "aperturaMx"), cos(radians(18.f)));
		glUniform1f(glGetUniformLocation(focalLight.Program, "aperturaMn"), cos(radians(12.f)));*/
		
		glm::mat4 view;
		view = camara->LookAt();
	
		objeto2->Rotate(rotacion);
		objeto2->Move(mov);
		
		mat4 projection;
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		GLint modelLoc = glGetUniformLocation(pointLight.Program, "model");
		GLint viewLoc = glGetUniformLocation(pointLight.Program, "view");
		GLint projLoc = glGetUniformLocation(pointLight.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		mat4 model2 = objeto2->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model2));
		objeto2->Draw();

		//CUBO PEQUEÑO, LAMPARA
		shaderLight.Use();
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		 modelLoc = glGetUniformLocation(focalLight.Program, "model");
		 viewLoc = glGetUniformLocation(focalLight.Program, "view");
		 projLoc = glGetUniformLocation(focalLight.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		mat4 model = objeto->GetModelMatrix();
		model = glm::translate(model, lightPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto->Draw();

		// CARGAR MODELO 3D
		modelShader.Use();
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		view = camara->LookAt();
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//modelo.Draw(modelShader);
	
		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	GLint a = glfwGetKey(window, GLFW_KEY_UP);
	GLint b = glfwGetKey(window, GLFW_KEY_DOWN);
	GLint c = glfwGetKey(window, GLFW_KEY_RIGHT);
	GLint d = glfwGetKey(window, GLFW_KEY_LEFT);
	GLint e = glfwGetKey(window, GLFW_KEY_KP_2);
	GLint f = glfwGetKey(window, GLFW_KEY_KP_4);
	GLint g = glfwGetKey(window, GLFW_KEY_KP_6);
	GLint h = glfwGetKey(window, GLFW_KEY_KP_8);


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (e == 1) {
		rotacion -= vec3(1.0, 0.0, 0.0);
	}
	else if (f == 1) {
		rotacion -= vec3(0.0, 1.0, 0.0);
	}
	else if (g == 1) {
		rotacion += vec3(0.0, 1.0, 0.0);
	}
	else if (h == 1) {
		rotacion += vec3(1.0, 0.0, 0.0);
	}
	if (a == 1) {
		mov += vec3(0.0, 0.1, 0.0);
	}
	else if (b == 1) {
		mov -= vec3(0.0, 0.1, 0.0);
	}
	else if (c == 1) {
		mov += vec3(0.1, 0.0, 0.0);
	}
	else if (d == 1) {
		mov -= vec3(0.1, 0.0, 0.0);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camara->MouseScroll(window, xoffset, yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	camara->MouseMove(window, xpos, ypos);
}
