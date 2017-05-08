#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <SOIL.h>

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
#include "Light.h"
using namespace glm;
using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Object *lampara1;
Object *lampara2;
Object *lampara3;
Object *lampara4;
Object *lampara5;
Object *cubo1; 
Object *cuboGrande;
Material *material;
Camera *camara;


const GLuint WIDTH = 800, HEIGHT = 800;

vec3 luz1(0.0, 0.0, 0.0);
vec3 luz2(-2, 0.0, 0.0);
vec3 luz3(-1, 0.7, 0.0);
vec3 luz4(0, -0.4, 0.0);
vec3 luz5(1, 0.7, 0);
vec3 posicionCubo(0.0, -3.0, 0.0);

vec3 lightDir(0.0, 0.0, -1.0);


vec3 lightFocDir(0.0, -1.0, 0.0);

vec3 color1(1.0);
vec3 color2(1.0, 0.0, 1.0);
vec3 color3(0.0, 1.0, 0.0);
vec3 color4(1, 1, 1.0);
vec3 color5(0.0, 1.0, 1.0);
vec3 rotacion;
vec3 mov;
int m1 = 0;
int m2 = 0;
int m3 = 0;
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

	Shader shaderLight("./src/LightVertexShader.vertexshader", "./src/LightFragmentShader.fragmentshader");
	Shader pared("./src/dirLight.vertexshader", "./src/pared.fragmentshader");
	Shader shader("./src/vertex.vertexshader", "./src/fragment.fragmentshader");
	
	Shader modelShader("./src/modelShader.vertexshader", "./src/modelShader.fragmentshader");
	Model modelo("./src/spider/spider.obj");
	Model modelo2("./src/spider/empty_mat.obj");
	Model modelo3("./src/spider/WusonOBJ.obj");


	lampara1 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(-5.0, 0.0,0.0), Object::cube);//lampara
	lampara2 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(luz2.x, luz2.y, luz2.z), Object::cube);
	lampara3 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(luz3.x, luz3.y, luz3.z), Object::cube);
	lampara4 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(luz4.x, luz4.y, luz4.z), Object::cube);
	lampara5 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(luz5.x, luz5.y, luz5.z), Object::cube);
	cubo1 = new Object(vec3(0.3f), vec3(0.0, 0.0, 0.0), posicionCubo, Object::cube);//cubo grande
	cuboGrande = new Object(vec3(5.f, 3.f, 8.f), vec3(0.0, 0.0, 0.0), vec3(0.0), Object::cube);//cubo grande
	camara = new Camera(vec3(0.0f, 0.0f, 1.0f), vec3(0.0), 0.05, 45.0);
	material = new Material("./src/difuso.png", "./src/especular.png", 200.f);
	Light directional(luz1, lightDir, vec3(0.2f), color1, vec3(1.f), vec3(1.f), Light::DIRECTIONAL, 1);
	Light puntual(luz2, lightDir, vec3(0.2f), color2, vec3(1.0), vec3(1.0), Light::POINT, 0);
	Light puntual2(luz4, lightDir, vec3(0.2f), color3, vec3(2.0), vec3(2.0), Light::POINT, 1);
	Light focal(luz3, lightFocDir, vec3(0.2f), color4, vec3(10.0), vec3(10.0), Light::SPOT, 1);
	Light focal2(luz5, lightFocDir, vec3(0.2f),color5, vec3(10.0), vec3(10.0), Light::SPOT, 0);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
	
		glfwPollEvents();
		camara->DoMovement(window);

		glClearColor(0.0f, 0.f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		shader.Use();
		material->ActivateTextures();
		material->SetMaterial(&shader);
		material->SetShininess(&shader);
		vec3 posCam = camara->posicionCamara();
		directional.SetDirection(lightDir);
		directional.SetLight(&shader, posCam);

		puntual.SetAtt(1.0f, 0.22f, 0.20f);
		puntual.SetLight(&shader, posCam);

		puntual2.SetAtt(1.0f, 0.22f, 0.20f);
		puntual2.SetLight(&shader, posCam);

		focal.SetAtt(1.0, 0.09, 0.032);
		focal.SetLight(&shader, posCam);
		focal.SetAperture(12.f, 20.f);

		focal2.SetAtt(1.0, 0.09, 0.032);
		focal2.SetLight(&shader, posCam);
		focal2.SetAperture(12.f, 20.f);
		
		glm::mat4 view;
		view = camara->LookAt();
	
		cubo1->Rotate(rotacion);
		cubo1->Move(mov);
		
		mat4 projection;
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		mat4 model = cubo1->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		cubo1->Draw();
		mat4 modelpared = cuboGrande->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelpared));
		cuboGrande->Draw();



		//CUBO PEQUEÑO, LAMPARA
		shaderLight.Use();
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		 modelLoc = glGetUniformLocation(shaderLight.Program, "model");
		 viewLoc = glGetUniformLocation(shaderLight.Program, "view");
		 projLoc = glGetUniformLocation(shaderLight.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		mat4 model1 = lampara1->GetModelMatrix();
		glUniform3f(glGetUniformLocation(shaderLight.Program, "Color"), color1.x,color1.y, color1.z);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

		lampara1->Draw();

		mat4 model2 = lampara2->GetModelMatrix();
		glUniform3f(glGetUniformLocation(shaderLight.Program, "Color"), color2.x, color2.y, color2.z);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

		lampara2->Draw();

		mat4 model3 = lampara3->GetModelMatrix();
		glUniform3f(glGetUniformLocation(shaderLight.Program, "Color"), color4.x, color4.y, color4.z);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));

		lampara3->Draw();

		mat4 model4 = lampara4->GetModelMatrix();
		glUniform3f(glGetUniformLocation(shaderLight.Program, "Color"), color3.x, color3.y, color3.z);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));

		lampara4->Draw();

		mat4 model5 = lampara5->GetModelMatrix();
		glUniform3f(glGetUniformLocation(shaderLight.Program, "Color"), color5.x, color5.y, color5.z);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));

		lampara5->Draw();

	/*	pared.Use();
		material->ActivateTextures();
		material->SetMaterial(&pared);
		material->SetShininess(&pared);
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		modelLoc = glGetUniformLocation(shaderLight.Program, "model");
		viewLoc = glGetUniformLocation(shaderLight.Program, "view");
		projLoc = glGetUniformLocation(shaderLight.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		mat4 modelpared = cuboGrande->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelpared));
		cuboGrande->Draw();*/


		// CARGAR MODELO 3D
		modelShader.Use();

		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		view = camara->LookAt();
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		mat4 model6;
		
		if (m1 == 1) {
			model6 = glm::translate(model6, glm::vec3(0.0f, -1.f, -3.0f));
			model6 = glm::scale(model6, glm::vec3(0.005f, 0.005f,0.005f));
			glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model6));
			modelo.Draw(modelShader);
		}
		else if (m2 == 1) {
			model6 = glm::translate(model6, glm::vec3(0.0f, -1.f, -3.0f));
			model6 = glm::scale(model6, glm::vec3(0.09f, 0.09f, 0.09f));
			glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model6));
			modelo2.Draw(modelShader);
		}
		else if (m3 == 1) {
			model6 = glm::translate(model6, glm::vec3(0.0f, -1.f, -3.0f));
			model6 = glm::scale(model6, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model6));
			modelo3.Draw(modelShader);
		}
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
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		m1 = 1;
		m2 = 0;
		m3 = 0;
	}	
	else if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		m2 = 1;
		m1 = 0;
		m3 = 0;
	}	
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		m3 = 1;
		m1 = 0;
		m2 = 0;
	}
		
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camara->MouseScroll(window, xoffset, yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	camara->MouseMove(window, xpos, ypos);
}
