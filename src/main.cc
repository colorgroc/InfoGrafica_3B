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
vec3 luz2(0.5, 0.0, 0.0);
vec3 luz3(1.0, 0.0, 0.0);
vec3 luz4(1.5, 0.0, 0.0);
vec3 luz5(2.0, 0.0, 0.0);
vec3 posicionCubo(0.0, -3.0, 0.0);

//glm::vec3 lightDirPos(0.f, 1.5f, 0.f);
vec3 lightDir(1.0, 0.0, 0.0);

//vec3 lightFocPos(0.f, 1.5f, 0.f);
vec3 lightFocDir(0.0, 1.0, 0.0);

//vec3 lightPointPos(1.f, 1.5f, 0.f);

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
	/*Shader ambientLight("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	Shader dirLight("./src/dirLight.vertexshader", "./src/dirLight.fragmentshader");
	Shader pointLight("./src/pointLight.vertexshader", "./src/pointLight.fragmentshader");
	Shader focalLight("./src/focalLight.vertexshader", "./src/focalLight.fragmentshader");*/
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
	cuboGrande = new Object(vec3(5.f), vec3(0.0, 0.0, 0.0), vec3(0.0), Object::cube);//cubo grande
	camara = new Camera(vec3(0.0f, 0.0f, 1.0f), vec3(0.0), 0.05, 45.0);
	material = new Material("./src/difuso.png", "./src/especular.png", 200.f);
	Light directional(luz1, lightDir, vec3(0.2f), vec3(1.0f), vec3(0.5f), vec3(0.5f), Light::DIRECTIONAL, 1);
	Light puntual(luz2, lightDir, vec3(0.2f), vec3(1.0f), vec3(0.5f), vec3(0.5f), Light::POINT, 1);
	Light focal(luz3, lightFocDir, vec3(0.5f), vec3(1.0f), vec3(1.0f), vec3(1.0f), Light::SPOT, 1);


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
		/*pointLight.Use();
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
		glUniform1f(glGetUniformLocation(pointLight.Program, "quadratic"), 0.032f);*/
		//luz focal
		/*	focalLight.Use();
		material->ActivateTextures();
		material->SetMaterial(&focalLight);
		material->SetShininess(&focalLight);
		vec3 posicion = camara->posicionCamara();
		GLint objectColorLoc = glGetUniformLocation(focalLight.Program, "cubeColor");
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
		glUniform3f(glGetUniformLocation(focalLight.Program, "lightDirection"), 0.0f, -1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(focalLight.Program, "aperturaMx"), cos(radians(25.f)));
		glUniform1f(glGetUniformLocation(focalLight.Program, "aperturaMn"), cos(radians(12.f)));*/
		
		shader.Use();
		//light.SetAtt()
		material->ActivateTextures();
		material->SetMaterial(&shader);
		material->SetShininess(&shader);
		vec3 posCam = camara->posicionCamara();
		focal.SetAperture(12.f, 20.f);
		focal.SetAtt(1.0, 0.09, 0.032);
		focal.SetLight(&shader, posCam);
		//directional.SetDirection(lightDir);
		//directional.SetLight(&shader, posCam);
		//puntual.SetAtt(1.0f, 0.09f, 0.032f);
		//puntual.SetLight(&shader, posCam);

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

		//CUBO PEQUEÑO, LAMPARA
		shaderLight.Use();
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		 modelLoc = glGetUniformLocation(shaderLight.Program, "model");
		 viewLoc = glGetUniformLocation(shaderLight.Program, "view");
		 projLoc = glGetUniformLocation(shaderLight.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		mat4 model1 = lampara1->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

		lampara1->Draw();

		mat4 model2 = lampara2->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

		lampara2->Draw();

		mat4 model3 = lampara3->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));

		lampara3->Draw();

		mat4 model4 = lampara4->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));

		lampara4->Draw();

		mat4 model5 = lampara5->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));

		lampara5->Draw();

		pared.Use();
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
		cuboGrande->Draw();


		// CARGAR MODELO 3D
		modelShader.Use();

		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		view = camara->LookAt();
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		mat4 model6;
		model6 = glm::translate(model6, glm::vec3(0.0f, -2.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model6 = glm::scale(model6, glm::vec3(0.8f, 0.8f, 0.8f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		if (m1 == 1) {
			modelo.Draw(modelShader);
		}
		else if (m2 == 1) {
			modelo2.Draw(modelShader);
		}
		else if (m3 == 1) {
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
