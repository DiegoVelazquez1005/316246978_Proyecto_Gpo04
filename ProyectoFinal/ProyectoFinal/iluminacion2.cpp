#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 12.0f, 20.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active = true;
bool iluminacion; 
bool switchI;
float rot = 0.0f, rotLapm = 0.0, CajTC = 0.0, CajTD = 0.0, CajTI = 0.0, SillaT = 0.0, rotLamp2 = 0.0;
bool limite = true, cajC = false, cajD = false, cajI = false, sillaActive = false, lmpActive = true;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-12.0f,9.9f, -2.0f),
	glm::vec3(-12.0f,9.9f, 0.0f),
	glm::vec3(-12.0f,9.9f, 2.0f)//,
	//glm::vec3(0.0f,12.0f, 5.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0.05, 0.05, 0.05);
glm::vec3 encender = glm::vec3(0.0, 0.0, 0.0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Iluminacion 2", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lightingShader2("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lightingShader3("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	
	Model cuarto((char*)"Models/cuarto/cuarto.obj");

	Model empotrado((char*)"Models/cuarto/empotrado.obj");

	Model lampara((char*)"Models/cuarto/lampara.obj");

	Model cristales((char*)"Models/cristales/cristales.obj");
	Model fachada((char*)"Models/fachada/fachada.obj");
	Model cama((char*)"Models/cama/cama.obj");
	Model planta((char*)"Models/planta/planta.obj");
	Model palo((char*)"Models/planta/planta2.obj");
	Model calefactor((char*)"Models/calefactor/calefactor.obj");
	Model librero((char*)"Models/librero/librero.obj");
	Model silla((char*)"Models/silla/silla.obj");
	Model escritorio((char*)"Models/escritorio/escritorio.obj");
	Model cajonD((char*)"Models/escritorio/cajonD.obj");
	Model cajonC((char*)"Models/escritorio/cajonC.obj");
	Model cajonI((char*)"Models/escritorio/cajonI.obj");
	Model computadora((char*)"Models/computadora/computadora.obj");
	Model puerta((char*)"Models/puerta/puerta.obj");




	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"),1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		
		//Load Model
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.01f,0.01f,0.01f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.4f, 0.4f, 0.4f);


		// Point light 1
	    glm::vec3 lightColor;
		glm::vec3 spotLigthActive;
		glm::vec3 spotLightRot;
		/*lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);*/
		lightColor = Light1;
		spotLigthActive = encender;
		spotLightRot.x = sin(glfwGetTime() * 0.5f) * rotLapm;
		spotLightRot.y = -1.0;
		spotLightRot.z = 0.0;

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

		//// Point light 4
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor.x, lightColor.y, lightColor.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].position"), -12.0f, 10.0f, -2.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].direction"), -spotLightRot.x, spotLightRot.y, spotLightRot.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].ambient"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].diffuse"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].specular"),1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].linear"), 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].quadratic"), 0.04f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].position"), -12.0f, 10.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].direction"), spotLightRot.x, spotLightRot.y, spotLightRot.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].ambient"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].diffuse"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].linear"), 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].quadratic"), 0.04f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].position"), -12.0f, 10.0f, 2.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].direction"), -spotLightRot.x, spotLightRot.y, spotLightRot.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].ambient"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].diffuse"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].linear"), 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].quadratic"), 0.04f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].outerCutOff"), glm::cos(glm::radians(15.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

	

		//Carga de modelos normales
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cuarto.Draw(lightingShader);

		//::::::::::EMPOTRADO::::::::::
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		empotrado.Draw(lightingShader);

		//::::::::::LAMPARA1::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.0f, 10.0f, -2.0f));
		model = glm::rotate(model, glm::radians(-rotLamp2) , glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		lampara.Draw(lightingShader);

		//::::::::::LAMPARA2::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.0f, 10.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotLamp2), glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		lampara.Draw(lightingShader);

		//::::::::::LAMPARA3::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.0f, 10.0f, 2.0f));
		model = glm::rotate(model, glm::radians(-rotLamp2), glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		lampara.Draw(lightingShader);

		//::::::::::PUERTA::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.125f, 6.95f, -0.665f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		puerta.Draw(lightingShader);

		//::::::::::FACHADA::::::::::
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		fachada.Draw(lightingShader);

		//::::::::::PLANTA 1::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.5f, 9.49f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		planta.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.5f, 9.49f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		palo.Draw(lightingShader);

		//::::::::::PLANTA 2::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 9.49f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		planta.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 9.49f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		palo.Draw(lightingShader);

		//::::::::::PLANTA 3::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 9.49f, -0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		planta.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 9.49f, -0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		palo.Draw(lightingShader);

		//::::::::::CAMA::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.925f, -4.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cama.Draw(lightingShader);

		//::::::::::CALEFACTOR::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.75f, 5.73f, -2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		calefactor.Draw(lightingShader);

		//::::::::::LIBRERO::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.3f, 7.2f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		librero.Draw(lightingShader);

		//::::::::::ESCRITORIO::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.9f, 5.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		escritorio.Draw(lightingShader);

		//::::::::::CAJOND::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.9f, 5.2f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, CajTD));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cajonD.Draw(lightingShader);

		//::::::::::CAJONC::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.9f, 5.2f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, CajTC));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cajonC.Draw(lightingShader);

		//::::::::::CAJONI::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.9f, 5.2f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, CajTI));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cajonI.Draw(lightingShader);

		//::::::::::SILLA::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 6.1f, 4.8f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, SillaT));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		silla.Draw(lightingShader);

		//::::::::::COMPUTADORA::::::::::
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.8f, 6.95f, 5.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		computadora.Draw(lightingShader);

		//Con transparencia

		//::::::::::CRISTALES::::::::::
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.999f, 0.999f, 0.999f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.75);

		cristales.Draw(lightingShader);



		glUniform4f(glGetUniformLocation(lightingShader.Program, "material.color"), 1.0, 1.0, 1.0, 5.0);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//hojas.Draw(lightingShader);

		glBindVertexArray(0);

	

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 3; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.01f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

	if (keys[GLFW_KEY_1] && rot <= -90) {
		limite = true;
	}
	if (keys[GLFW_KEY_1] && rot >= 0) {
		limite = false;
	}
	if ((limite) && (rot <= 0)) {
		rot += 0.5;
	}
	if ((!limite) && (rot >= -90)) {
		rot -= 0.5;
	}

	

	if (lmpActive) {
		rotLamp2 = sin(glfwGetTime() * 0.5f) * 25.0;
	}
	if (!lmpActive) {
		rotLamp2 = 0 ;
	}






	if (keys[GLFW_KEY_3] && CajTI <= -0.8) {
		cajI = false;
	}
	if (keys[GLFW_KEY_3] && CajTI >= 0) {
		cajI = true;
	}
	if ((cajI) && (CajTI >= -0.8)) {
		CajTI -= 0.01;
	}
	if ((!cajI) && (CajTI <= 0.0)) {
		CajTI += 0.01;
	}

	if (keys[GLFW_KEY_4] && CajTC <= -0.8) {
		cajC = false;
	}
	if (keys[GLFW_KEY_4] && CajTC >= 0) {
		cajC = true;
	}
	if ((cajC) && (CajTC >= -0.8)) {
		CajTC -= 0.01;
	}
	if ((!cajC) && (CajTC <= 0.0)) {
		CajTC += 0.01;
	}

	if (keys[GLFW_KEY_5] && CajTD <= -0.8) {
		cajD = false;
	}
	if (keys[GLFW_KEY_5] && CajTD >= 0) {
		cajD = true;
	}
	if ((cajD) && (CajTD >= -0.8)) {
		CajTD -= 0.01;
	}
	if ((!cajD) && (CajTD <= 0.0)) {
		CajTD += 0.01;
	}

	if (keys[GLFW_KEY_6] && SillaT <= -0.8) {
		sillaActive = false;
	}
	if (keys[GLFW_KEY_6] && SillaT >= 0) {
		sillaActive = true;
	}
	if ((sillaActive) && (SillaT >= -0.8)) {
		SillaT -= 0.01;
	}
	if ((!sillaActive) && (SillaT <= 0.0)) {
		SillaT += 0.01;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_2])
	{
		if (active)
		{
			Light1 = glm::vec3(0.05, 0.05, 0.05);
			encender = glm::vec3(0.0, 0.0, 0.0);
			rotLapm = 0.0;
			active = false;
		}
		else
		{	

			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
			encender = glm::vec3(1.0, 1.0, 1.0);
			rotLapm = 0.25;
			active = true;

		}
	}
	if (keys[GLFW_KEY_2]) {
		lmpActive = !lmpActive;
	}
	
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}