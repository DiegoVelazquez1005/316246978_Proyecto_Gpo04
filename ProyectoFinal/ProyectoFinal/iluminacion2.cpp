#include <iostream>
#include <cmath>
#include <windows.h>

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
void animacion();

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

//animación c 1
float tienpo = 0.0f;
float speed = 0.0f;

float coordU = 0;
float coordV = 0;

bool animate1 = false;
bool animate2 = false;

//ANIMACIÓN c2
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
// Keyframes
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z, cRotoX, cRotoY;

#define MAX_FRAMES 12
int i_max_steps = 90;
int i_curr_steps = 0;
float activaCristalRoto = 0.8f, activaCristal = 0.8f;;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;
bool cargaDeDatos = true;

void saveFrame(void)
{
	float x = 0.0f, y;

	for (int i = 0; i < 10; i++) {
		if (i < 8) {
			//printf("1----i %d\n", i);
			//printf("frameindex %d\n", FrameIndex);
			KeyFrame[i].posX = x;
			y = (tan(80) * x) - ((10 / (2 * pow(18, 2) * pow(cos(80), 2))) * pow(x, 2));
			KeyFrame[i].posY = y;
			//printf("Se almacena el valor de  %f en X*\n", x);
			//printf("Se almacena el valor de  %f en Y*\n", y);
			if (i < 7) {
				x++;
				//printf("1----x %f\n", x);
			}
			else {
				x = 0.0f;
			}
			
			
		}
		if (i >= 7) {
			
			//printf("i %d\n", i);
			KeyFrame[i].posX = x+7;
			//printf("POS X** %f\n", KeyFrame[i].posX);
			//printf("Se almacena el valor de  %f en X-----\n", x);
			y = (tan(80) * (x)) - ((10 / (2 * pow(7, 2) * pow(cos(80), 2))) * pow((x), 2));
			KeyFrame[i].posY = y;
			//printf("Se almacena el valor de  %f en Y*\n", y);
			x += 0.542;
			//FrameIndex++;
		}
		
		FrameIndex++;
	}
	////x = 0;
	////for (int i = 8; i < 11; i++) {

	////	

	////}

}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	//printf("reseteo de posicion en x %f\n", posX);
	posY = KeyFrame[0].posY;
	//printf("reseteo de posicion en y %f\n", posY);
	posZ = KeyFrame[0].posZ;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;


}


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

GLfloat vertices2[] =
{
	// Positions            // Normals              // Texture Coords
	-0.5f, -0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.0f + coordU,  0.75f + coordV,
	0.5f, -0.5f, 0.0f,     0.0f,  0.0f, -1.0f,     0.25f + coordU, 0.75f + coordV,
	0.5f,  0.5f, 0.0f,     0.0f,  0.0f, -1.0f,     0.25f + coordU, 1.0f + coordV,
	0.5f,  0.5f, 0.0f,     0.0f,  0.0f, -1.0f,     0.25F + coordU, 1.0f + coordV,
	-0.5f,  0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.0f + coordU,  1.0f + coordV,
	-0.5f, -0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.0f + coordU,  0.75f + coordV,


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
	Shader animCompleja("Shaders/animCompleja.vs", "Shaders/animCompleja.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	
	Model cuarto((char*)"Models/cuarto/cuarto.obj");

	Model empotrado((char*)"Models/cuarto/empotrado.obj");

	Model lampara((char*)"Models/cuarto/lampara.obj");

	Model cristales((char*)"Models/cristales/cristales.obj");
	Model cristal((char*)"Models/cristales/cristal.obj");
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

	Model pelota((char*)"Models/pelota/pelota.obj");

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
	}




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

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	GLuint  VAOanim, VBOanim;
	glGenVertexArrays(1, &VAOanim);
	glGenBuffers(1, &VBOanim);
	glBindVertexArray(VAOanim);
	glBindBuffer(GL_ARRAY_BUFFER, VBOanim);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	 //Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	 //Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Load textures
	GLuint texture1;
	glGenTextures(1, &texture1);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image = stbi_load("images/animacion.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);






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
		animacion();

		// Clear the colorbuffer
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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


		//::::::::::PELOTA::::::::::
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX-20.6f, posY+5.3f, 0.0f));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pelota.Draw(lightingShader);


		//Con transparencia

		//::::::::::CRISTALES::::::::::
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.999f, 0.999f, 0.999f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.8);
		cristales.Draw(lightingShader);



		/*model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.999f, 0.999f, 0.999f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, activaCristal);
		cristal.Draw(lightingShader);*/


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



		animCompleja.Use();
		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		viewPosLoc = glGetUniformLocation(animCompleja.Program, "viewPos");


		tienpo = glfwGetTime() * speed ;
		modelLoc = glGetUniformLocation(animCompleja.Program, "model");
		viewLoc = glGetUniformLocation(animCompleja.Program, "view");
		projLoc = glGetUniformLocation(animCompleja.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform1f(glGetUniformLocation(animCompleja.Program, "tienpo"), tienpo);
		glUniform1f(glGetUniformLocation(animCompleja.Program, "texture1"), 0);
		glBindVertexArray(VAOanim);

		model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(-12.48f, 6.98f, 5.1f));
		
		
		model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.61f, 0.437f, 0.0f));
		//glUniform1i(glGetUniformLocation(animCompleja.Program, "activaTransparencia"), 0);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2f(glGetUniformLocation(animCompleja.Program, "incCoordUV"), coordU, coordV);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);





		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAOanim);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

void animacion()
{

	//Movimiento del personaje

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation

			posX += KeyFrame[playIndex].incX;
			//printf("valor en x %f\n", posX);
			posY += KeyFrame[playIndex].incY;
			//printf("valor en y %f\n", posY);

			/*if (posY < 12 && posX <= 7 ) {
				cRotoY = posY;
				activaCristalRoto = 0.8;
			}
			else {
				activaCristalRoto = 0.0;
			}*/

			if (posY < 12 && posX > 6) {
				activaCristal = 0.0f;
			}
			else {
				activaCristal = 0.8f;
			}
			i_curr_steps++;
		}

	}
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




	if (animate1) {
		if (coordU < 1.0f) {
			//Sleep(150);
			coordU += 0.25f;
		}
		else {
			coordU = 0.0f;
			coordV -= 0.25;
			if (coordU < -1.0) {
				coordV = 0.0f;
			}
		}
	}
	else {
		coordU = 0.0f;
		coordV = 0.0f;
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

	if (keys[GLFW_KEY_I])
	{
		speed += 1.0;
	}
	if (keys[GLFW_KEY_K])
	{
		speed -= 1.0;
	}

	if (keys[GLFW_KEY_8]) {
		animate1 = !animate1;
	}

	if (keys[GLFW_KEY_7])
	{
		if (cargaDeDatos) {
			saveFrame();
			cargaDeDatos = false;
		}


		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

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