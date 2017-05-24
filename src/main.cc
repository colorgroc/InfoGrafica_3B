#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <chrono>

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
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
GLuint loadTexture(GLchar* path);

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
int postProcessing = 0;

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

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	glDepthFunc(GL_LESS); ///ESTO Q ES??!
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);

	Shader shaderLight("./src/LightVertexShader.vertexshader", "./src/LightFragmentShader.fragmentshader");
	Shader pared("./src/dirLight.vertexshader", "./src/pared.fragmentshader");
	Shader shader("./src/vertex.vertexshader", "./src/fragment.fragmentshader");
	
	Shader modelShader("./src/modelShader.vertexshader", "./src/modelShader.fragmentshader");
	Model modelo("./src/SoccerBall/Soccer Ball.obj");
	Model modelo2("./src/estadioFutbol/estadio.obj");

	Shader screenShader("./src/screenVertx.vertexshader", "./src/screenFrag.fragmentshader");

	//lampara1 = new Object(vec3(0.1f), vec3(0.0f, 0.5f, 0.0f), vec3(-5.0, 0.0, 0.0), Object::cube);//lampara
	lampara2 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(luz2.x, luz2.y, luz2.z), Object::cube);
	lampara3 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(luz3.x, luz3.y, luz3.z), Object::cube);
	lampara4 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(luz4.x, luz4.y, luz4.z), Object::cube);
	lampara5 = new Object(vec3(0.1f), vec3(0.0f, 0.0f, 0.0f), vec3(luz5.x, luz5.y, luz5.z), Object::cube);
	
	
	cubo1 = new Object(vec3(0.3f), vec3(0.0, 0.0, 0.0), posicionCubo, Object::cube);//cubo grande
	cuboGrande = new Object(vec3(5.f, 3.f, 8.f), vec3(0.0, 0.0, 0.0), vec3(0.0), Object::cube);//cubo grande
	Light directional(luz1, lightDir, vec3(0.4f), color1, vec3(1.f), vec3(1.f), Light::DIRECTIONAL, 1);
	Light puntual(luz2, lightDir, vec3(0.2f), color2, vec3(2.0), vec3(2.0), Light::POINT, 0);
	Light puntual2(luz4, lightDir, vec3(0.2f), color3, vec3(2.0), vec3(2.0), Light::POINT, 1);
	Light focal(luz3, lightFocDir, vec3(0.2f), color4, vec3(10.0), vec3(10.0), Light::SPOT, 1);
	Light focal2(luz5, lightFocDir, vec3(0.2f), color5, vec3(10.0), vec3(10.0), Light::SPOT, 0);
	camara = new Camera(vec3(0.0f, 0.0f, 1.0f), vec3(0.0), 0.05, 45.f);


	GLfloat quadVertices[] = {  //se crea un quad el cual ocupara toda la pantalla
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//FRAMEBUFFER AQUI
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Se atacha la textura al framebuffer
	//GLuint textureColorbuffer = generateAttachmentTexture(false, false);
	GLuint textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// se crea un render buffer object, es util ya que solo queremos el color buffer
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	// Ahora ya esta creado el framebuffer y estan atachados sus valores

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();	
		camara->DoMovement(window);
		//INICIALIZAR FRAMEBUFFER
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// Clear all attached buffers        
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //no limpiamos el stencil buffer pq no o utilizamos

		glEnable(GL_DEPTH_TEST);

		//llamamaos al shader con el cual aplicaremos las luzes
		shader.Use();
		//GLuint pelotaTex = loadTexture("./src/difuso.png"); //se asigna la textura cargada
		//glUniform1i(glGetUniformLocation(shader.Program, "tex"), 0);

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
		glm::mat4 model;


		mat4 view = camara->LookAt();
		mat4 projection = perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, value_ptr(projection));
		
		cubo1->Rotate(rotacion);
		cubo1->Move(mov);
		model = cubo1->GetModelMatrix();
		
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, value_ptr(model));
		
		cubo1->Draw();
		
		modelShader.Use();
		mat4 model7;
		model7 = glm::translate(model7, glm::vec3(0.0f, -1.f, 0.0f));
		model7 = glm::scale(model7, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model7));
		modelo2.Draw(modelShader);

		//llamamos al shader para que pinte las lamapras, simplemente son cubos de un color
		shaderLight.Use();
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		GLuint  modelLoc = glGetUniformLocation(shaderLight.Program, "model");
		GLuint viewLoc = glGetUniformLocation(shaderLight.Program, "view");
		GLuint projLoc = glGetUniformLocation(shaderLight.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

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


		modelShader.Use();
		projection = glm::perspective(camara->GetFOV(), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		view = camara->LookAt();
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//mat4 model6;

		//RESTABLECER FRAMEBUFFER
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Clear all relevant buffers
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
							  
		//DIBUJADO EN LA PANTALLA DE QUADS DEL BUFFER, todo lo que se ha pintado anteriormente se pinta en el quad
		screenShader.Use();
		glUniform1i(glGetUniformLocation(screenShader.Program, "postProcessing"), postProcessing);
		glUniform1f(glGetUniformLocation(screenShader.Program, "gamma"), 0.5f);
	
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);// pintamos con el color atachado de la texutra anteriormente
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//los modelos se pintan despues de que se pinte el quad, por lo que los postprocesos no les afectan
		// CARGAR MODELO 3D
		

		// Swap the buffers
		glfwSwapBuffers(window);
	}
	//ELIMINAR FRAMEBUFFER
	glDeleteFramebuffers(1, &framebuffer);

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

	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		postProcessing = 0;
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		postProcessing = 1;
	}
	else if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		postProcessing = 2;
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		postProcessing = 3;
	}
	else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		postProcessing = 4;
	}
	else if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		postProcessing = 5;
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camara->MouseScroll(window, xoffset, yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	camara->MouseMove(window, xpos, ypos);
}

GLuint loadTexture(GLchar* path)
{
	//se crea una textura y se le asigna la imagen deseada
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;

}

// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, WIDTH, HEIGHT, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		 //a diferencia de las texturas anteriores, el ultimo paramtro es NULL ya que se va a ir generando dinamicamente
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}