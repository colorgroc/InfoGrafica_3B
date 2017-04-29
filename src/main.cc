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
using namespace glm;
using namespace std;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void DoMovment(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
Object *objeto;
Object *objeto2; 

const GLuint WIDTH = 800, HEIGHT = 800;
bool textura = false;
bool up;
bool derecha;
float angulo;
float angulo1;
vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool der;
bool arriba;
bool firstMouse = true;
Camera *camara;
GLfloat camSpeed;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat lastX =  WIDTH  / 2.0;
GLfloat lastY =  HEIGHT / 2.0;
GLfloat yaww = -90.0f, pitchh = 0.0f;
GLfloat fov = 45.0f;
glm::vec3 lightPos(1.0f, 1.0f, 1.0f);

vec3 rotacion;
vec3 mov;
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

	Shader ourShader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	Shader shaderLight("./src/LightVertexShader.vertexshader", "./src/LightFragmentShader.fragmentshader");
	Shader modelShader("./src/modelShader.vertexshader", "./src/modelShader.fragmentshader");
	Model modelo("./src/spider/spider.obj");
	
	objeto = new Object(vec3(0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(lightPos.x, lightPos.y, lightPos.z), Object::cube);//lampara
	objeto2 = new Object(vec3(0.3f), vec3(0.0, 0.0, 0.0), vec3(0.5f, 1.0f, 1.0f), Object::cube);//cubo grande
	camara = new Camera(vec3(0.0f, 0.0f, 3.0f), vec3(0.0), 0.05, 45.0);

/*	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); 


	//TEXTURA
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("./src/hola.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); 

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width2, height2;
	unsigned char* image2 = SOIL_load_image("./src/texture.png", &width2, &height2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0); */


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
	
		glfwPollEvents();
		//DoMovment(window);
		camara->DoMovement(window);

		glClearColor(0.0f, 0.f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//CUBO GRANDE
		ourShader.Use();
		vec3 posicion = camara->posicionCamara();
		GLint objectColorLoc = glGetUniformLocation(ourShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(ourShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(ourShader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPos");
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 0.0f); 
		glUniform3f(viewPosLoc, posicion.x,posicion.y, posicion.z);
		glm::mat4 view;
		view = camara->LookAt();
		objeto2->Rotate(rotacion);
		objeto2->Move(mov);
		
		mat4 projection;
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		mat4 model2 = objeto2->GetModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model2));
		objeto2->Draw();

		//CUBO PEQUEÑO, LAMPARA
		shaderLight.Use();
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		 modelLoc = glGetUniformLocation(shaderLight.Program, "model");
		 viewLoc = glGetUniformLocation(shaderLight.Program, "view");
		 projLoc = glGetUniformLocation(shaderLight.Program, "projection");

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
	
		/*glBindVertexArray(VAO);

		for (GLuint i = 0; i < 10; i++)//MOVER LOS CUBOS
		{
	
			glm::mat4 model;
			model = translate(model, CubesPositionBuffer[i]);
			GLfloat angle = 50.0f * i;
			if (i != 0) {
				model = rotate(model, (GLfloat)glfwGetTime()* 1.f, vec3(1.0f, 1.0f, 0.0f));
			}
			if (i == 0) {
				if(up)
					model = rotate(model, radians(angulo), vec3(0.0f, 1.0f, 0.0f));
				else if(!up)
					model = rotate(model, radians(angulo), vec3(0.0f, -1.0f, 0.0f));
				if(derecha)
					model = rotate(model, radians(angulo1), vec3(1.0f, 0.0f, 0.0f));
				else if(!derecha)
					model = rotate(model, radians(angulo1), vec3(-1.0f, 0.0f, 0.0f));

			}
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);*/

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		textura = true;
	}
	else if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
		textura = false;	
	}
	if (key == GLFW_KEY_KP_2 && action == GLFW_REPEAT) {
		rotacion -= vec3(1.0, 0.0, 0.0);
	}
	else if (key == GLFW_KEY_KP_4 && action == GLFW_REPEAT) {
		rotacion -= vec3(0.0, 1.0, 0.0);
	}
	else if (key == GLFW_KEY_KP_6 && action == GLFW_REPEAT) {
		rotacion += vec3(0.0, 1.0, 0.0);
	}
	else if (key == GLFW_KEY_KP_8 && action == GLFW_REPEAT) {
		rotacion += vec3(1.0, 0.0, 0.0);
	}
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
		mov += vec3(0.0, 0.1, 0.0);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
		mov -= vec3(0.0, 0.1, 0.0);
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
		mov += vec3(0.1, 0.0, 0.0);
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
		mov -= vec3(0.1, 0.0, 0.0);
	}
}

void DoMovment(GLFWwindow* window) { //MOVER CAMARA
	GLint a = glfwGetKey(window, GLFW_KEY_A);
	GLint w = glfwGetKey(window, GLFW_KEY_W);
	GLint s = glfwGetKey(window, GLFW_KEY_S);
	GLint d = glfwGetKey(window, GLFW_KEY_D);
	
	if (a == 1) {
		cameraPos -= normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
		//objeto->Rotate(vec3(0.0, 1.0, 0.0));
	}
	if (w == 1) {
		cameraPos += camSpeed * cameraFront;
	}
	if (s == 1) {
		cameraPos -= camSpeed * cameraFront;
	}
	if (d == 1) {
		cameraPos += normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	/*if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;*/
	camara->MouseScroll(window, xoffset, yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	/*if (firstMouse) //GIRO DE LA CAMARA
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaww += xoffset;
	pitchh += yoffset;
	pitchh = clamp(pitchh, -89.f, 89.f);
	yaww = mod(yaww, 360.f);


	glm::vec3 front;
	front.x = cos(glm::radians(yaww)) * cos(glm::radians(pitchh));
	front.y = sin(glm::radians(pitchh));
	front.z = sin(glm::radians(yaww)) * cos(glm::radians(pitchh));
	cameraFront = glm::normalize(front);*/
	
	
	camara->MouseMove(window, xpos, ypos);
}
