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

// Funciones prototipo
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

//Atributos de pointights
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active = true; 

//Atributos animacion sencilla 1
float rot = 0.0f; 
bool limite = true;

//Atributos animacion sencilla 2
float rotSpotLight = 0.0, rotLamp = 0.0;
glm::vec3 encenderPointLight = glm::vec3(0.05, 0.05, 0.05);
glm::vec3 encenderSpotLight = glm::vec3(0.0, 0.0, 0.0);
bool activarLampara = false;

//Atributos animacion sencilla 3
float tCajCen = 0.0, tCajDer = 0.0, tCajIzq = 0.0, tSilla = 0.0;
bool  cajCen= false,cajDer = false, cajIzq = false, sill = false;


//Atributos aimación compleja 1
float tiempo = 0.0f, speed = 0.0f, coordU = 0, coordV = 0;
bool animate = false;

//Atributos aimación compleja 2
#define MAX_FRAMES 12
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z;
int i_max_steps = 90,  i_curr_steps = 0;


typedef struct _frame {
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ

}FRAME;


FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;		
bool play = false, cargaDeDatos = true;
int playIndex = 0;

//----------tiroParabolico----------
/*
	Calculo de corrdenadas correspondientes al tiro parabolico,
	almacenamiento en KeyFrame[]
*/

void tiroParabolico(void) {
	float x = 0.0f, y;
	for (int i = 0; i < 10; i++) {
		if (i < 8) {
			KeyFrame[i].posX = x;
			y = (tan(80) * x) - ((10 / (2 * pow(18, 2) * pow(cos(80), 2))) * pow(x, 2));
			KeyFrame[i].posY = y;
			if (i < 7)
				x++;
			else
				x = 0.0f;
		}
		if (i >= 7) {
			KeyFrame[i].posX = x+7;
			y = (tan(80) * (x)) - ((10 / (2 * pow(7, 2) * pow(cos(80), 2))) * pow((x), 2));
			KeyFrame[i].posY = y;
			x += 0.542;
		}
		FrameIndex++;
	}
}

//----------tiroParabolico----------
/*
	Reseteo de las posiciones
*/

void resetElements(void) {
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
}

//----------interpolation----------
/*
	Calculo de las interpolaciones
*/

void interpolation(void) {
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
}

//__________pointLightPositions[]__________
/*
	Arreglo de vectores con las posiciones de las pointLights
*/

glm::vec3 pointLightPositions[] = {
	glm::vec3(-12.0f,9.9f, -2.0f),
	glm::vec3(-12.0f,9.9f, 0.0f),
	glm::vec3(-12.0f,9.9f, 2.0f)
};

//__________vertices[]__________
/*
	Arreglo de vertices con coordenadas
	correspondientes a un cubo
*/

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

//__________vertices2[]__________
/*
	Arreglo de vertices con coordenadas
	correspondientes a un plano con coordenadas de textura
*/

GLfloat vertices2[] = {
	// Posición            // Normales              // Texture Coords
	-0.5f, -0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.0f  + coordU,  0.75f + coordV,
	 0.5f, -0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.25f + coordU,  0.75f + coordV,
	 0.5f,  0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.25f + coordU,  1.0f  + coordV,
	 0.5f,  0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.25F + coordU,  1.0f  + coordV,
	-0.5f,  0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.0f  + coordU,  1.0f  + coordV,
	-0.5f, -0.5f, 0.0f,    0.0f,  0.0f, -1.0f,     0.0f  + coordU,  0.75f + coordV,
};

///


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

//----------main----------
/*
	Función principal
*/

int main(){
	glfwInit(); //Inicialización de la ventana

	//Creación de una ventana GLFWwindow
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "316246978_ProyectoFinal_GPO04", nullptr, nullptr);

	//Manejo de error: creación de ventana
	if (nullptr == window){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	//Se dibuja la ventana
	glfwMakeContextCurrent(window);
	//Buffer acorde al tamao de la ventana
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit()){
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	//Definición de las dimeciones del viewPort
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Shaders
	/*
		Carga de los Shaders para manejo de iluminación con 
			Spotlight
			PointLight
		y uso de animación con mivimiento de texturas
	*/

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader animCompleja("Shaders/animCompleja.vs", "Shaders/animCompleja.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	//Modelos 
	/*
		Carga de modelos desde la carpeta Models
	*/
	
	//<<<<<<<<<<FACHADA>>>>>>>>>>
	Model fachada((char*)"Models/fachada/fachada.obj");
	Model contenedor((char*)"Models/contenedorAgua/contenedorAgua.obj");
	Model puertaExterior((char*)"Models/fachada/puertaExterior.obj");

	//<<<<<<<<<<CUARTO>>>>>>>>>>
	Model cuarto((char*)"Models/cuarto/cuarto.obj");
	Model empotrado((char*)"Models/cuarto/empotrado.obj");
	Model lampara((char*)"Models/cuarto/lampara.obj");
	Model cristales((char*)"Models/cristales/cristales.obj");
	Model puerta((char*)"Models/puerta/puerta.obj");

	//<<<<<<<<<<OBJETO 1: PLANTA>>>>>>>>>>
	Model hojas((char*)"Models/planta/planta.obj");
	Model planta((char*)"Models/planta/planta2.obj");

	//<<<<<<<<<<OBJETO 2: CAMA>>>>>>>>>>
	Model cama((char*)"Models/cama/cama.obj");

	//<<<<<<<<<<OBJETO 3: CALEFACTOR>>>>>>>>>>
	Model calefactor((char*)"Models/calefactor/calefactor.obj");

	//<<<<<<<<<<OBJETO 4: LIBRERO>>>>>>>>>>
	Model librero((char*)"Models/librero/librero.obj");

	//<<<<<<<<<<OBJETO 5: SILLA>>>>>>>>>>
	Model silla((char*)"Models/silla/silla.obj");

	//<<<<<<<<<<OBJETO 6: ESCRITORIO>>>>>>>>>>
	Model escritorio((char*)"Models/escritorio/escritorio.obj");
	Model cajonDer((char*)"Models/escritorio/cajonD.obj");
	Model cajonCen((char*)"Models/escritorio/cajonC.obj");
	Model cajonIzq((char*)"Models/escritorio/cajonI.obj");

	//<<<<<<<<<<OBJETO 7: COMPUTADORA>>>>>>>>>>
	Model computadora((char*)"Models/computadora/computadora.obj");
	
	//<<<<<<<<<<PELOTA>>>>>>>>>>
	Model pelota((char*)"Models/pelota/pelota.obj");

	//Inicialización de Frames 
	for (int i = 0; i < MAX_FRAMES; i++) {
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
	}

	// Creación de VBO y VAO
	GLuint VBO, VAO;
	//Configuración del VAO
	glGenVertexArrays(1, &VAO);
	//Configuración del VBO
	glGenBuffers(1, &VBO);
	//Enlazado de VBO y VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//Atributos de posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributos de las normales
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Creación de VBOanim y VAOanim con vertices de coordenadas de textura 
	GLuint  VAOanim, VBOanim;
	//Configuración del VAOanim
	glGenVertexArrays(1, &VAOanim);
	//Configuración del VBO
	glGenBuffers(1, &VBOanim);
	//Enlazado de VBOanim y VAOanim
	glBindVertexArray(VAOanim);
	glBindBuffer(GL_ARRAY_BUFFER, VBOanim);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//Atributos de posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Atributos de las normales
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//Atributos de las coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Termina proceso de enlazado de los VBO y VAO
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
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}else{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	//Definición de materiales 
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"),1);

	//Proyección
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	//Mientras no se cierre la ventana 
	while (!glfwWindowShouldClose(window)) {

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Revisión de eventos
		glfwPollEvents();
		DoMovement();
		animacion();

		//Color de fondo (valores normalizados)
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// **********Shader: LIGHTING**********
		lightingShader.Use();

		//Vista
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.x, lightPos.x);

		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);

		//Componente Ambiental
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.01f,0.01f,0.01f);
		//Componente Difusa
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		//Componente Especular
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.4f, 0.4f, 0.4f);


		// Point light 
	    glm::vec3 pointLightActive;
		pointLightActive = encenderPointLight;


		//Spot light
		glm::vec3 spotLigthActive;
		glm::vec3 spotLightRot;		
		spotLigthActive = encenderSpotLight;
		spotLightRot.x = sin(glfwGetTime() * 0.5f) * rotSpotLight;
		spotLightRot.y = -1.0;
		spotLightRot.z = 0.0;

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), pointLightActive.x, pointLightActive.y, pointLightActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), pointLightActive.x, pointLightActive.y, pointLightActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), pointLightActive.x, pointLightActive.y, pointLightActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), pointLightActive.x, pointLightActive.y, pointLightActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.075f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), pointLightActive.x, pointLightActive.y, pointLightActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), pointLightActive.x, pointLightActive.y, pointLightActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.075f);

		// SpotLight 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].position"), -12.0f, 10.0f, -2.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].direction"), -spotLightRot.x, spotLightRot.y, spotLightRot.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].ambient"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].diffuse"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].specular"),0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// SpotLight 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].position"), -12.0f, 10.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].direction"), spotLightRot.x, spotLightRot.y, spotLightRot.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].ambient"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].diffuse"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[1].outerCutOff"), glm::cos(glm::radians(15.0f)));

		// SpotLight 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].position"), -12.0f, 10.0f, 2.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].direction"), -spotLightRot.x, spotLightRot.y, spotLightRot.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].ambient"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].diffuse"), spotLigthActive.x, spotLigthActive.y, spotLigthActive.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[2].outerCutOff"), glm::cos(glm::radians(15.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 10.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Matrices de modelo, vista y proyección
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		//Matrices de vista y proyección a shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);
		//Carga de modelos

		//..........FACHADA..........
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		fachada.Draw(lightingShader);

		//..........CONTENEDOR..........
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		contenedor.Draw(lightingShader);

		//..........PUERTA EXTERIOR..........
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		puertaExterior.Draw(lightingShader);
		
		//..........CUARTO..........
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cuarto.Draw(lightingShader);

		//..........EMPOTRADO..........
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		empotrado.Draw(lightingShader);

		//..........LAMPARA1..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.0f, 10.0f, -2.0f));
		model = glm::rotate(model, glm::radians(-rotLamp) , glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		lampara.Draw(lightingShader);

		//..........LAMPARA2..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.0f, 10.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotLamp), glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		lampara.Draw(lightingShader);

		//..........LAMPARA3..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.0f, 10.0f, 2.0f));
		model = glm::rotate(model, glm::radians(-rotLamp), glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		lampara.Draw(lightingShader);

		//..........PUERTA..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.125f, 6.95f, -0.665f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		puerta.Draw(lightingShader);

		//..........PLANTA 1..........
		//HOJAS
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.5f, 9.49f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		hojas.Draw(lightingShader);
		//PLANTA
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.5f, 9.49f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		planta.Draw(lightingShader);

		//..........PLANTA 2..........
		//HOJAS
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 9.49f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		hojas.Draw(lightingShader);
		//PLANTA
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 9.49f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		planta.Draw(lightingShader);

		//..........PLANTA 3..........
		//HOJAS
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 9.49f, -0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		hojas.Draw(lightingShader);
		//PLANTA
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 9.49f, -0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		planta.Draw(lightingShader);

		//..........CAMA..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.925f, -4.25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cama.Draw(lightingShader);

		//..........CALEFACTOR..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.75f, 5.73f, -2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		calefactor.Draw(lightingShader);

		//..........LIBRERO..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.3f, 7.2f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		librero.Draw(lightingShader);

		//..........ESCRITORIO..........
		//ESCRITORIO
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.9f, 5.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		escritorio.Draw(lightingShader);

		//CAJONDER
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.9f, 5.2f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, tCajDer));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cajonDer.Draw(lightingShader);

		//CAJONCEN
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.9f, 5.2f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, tCajCen));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cajonCen.Draw(lightingShader);

		//CAJONIZQ
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 5.9f, 5.2f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, tCajIzq));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cajonIzq.Draw(lightingShader);

		//..........SILLA..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-13.0f, 6.1f, 4.8f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, tSilla));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		silla.Draw(lightingShader);

		//..........COMPUTADORA..........
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.8f, 6.95f, 5.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		computadora.Draw(lightingShader);


		//..........PELOTA..........
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX-20.6f, posY+5.3f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pelota.Draw(lightingShader);

		//Avtivacion de la funcionalidad para trabajar el canal alfa
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//..........CRISTALES..........
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.999f, 0.999f, 0.999f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.8);
		cristales.Draw(lightingShader);


		//glUniform4f(glGetUniformLocation(lightingShader.Program, "material.color"), 1.0, 1.0, 1.0, 5.0);
		glDisable(GL_BLEND);  //Desactivacion el canal alfa 

		glBindVertexArray(0);


		// **********Shader: LIGHTING**********
		lampShader.Use();

		//Matrices de modelo, vista y proyección
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Configuración de matrices de vista y proyección
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Ubicación de la luz
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Dibujo de pointLights
		for (GLuint i = 0; i < 3; i++) {
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//Desenlace de memoria
		glBindVertexArray(0);

		// **********Shader: animCompleja**********
		animCompleja.Use();
		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		viewPosLoc = glGetUniformLocation(animCompleja.Program, "viewPos");

		tiempo = glfwGetTime() * speed ;

		//Matrices de modelo, vista y proyección
		modelLoc = glGetUniformLocation(animCompleja.Program, "model");
		viewLoc = glGetUniformLocation(animCompleja.Program, "view");
		projLoc = glGetUniformLocation(animCompleja.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1f(glGetUniformLocation(animCompleja.Program, "tiempo"), tiempo);
		glUniform1f(glGetUniformLocation(animCompleja.Program, "texture1"), 0);
		glBindVertexArray(VAOanim);

		
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.48f, 6.98f, 5.1f));		
		model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.61f, 0.437f, 0.0f));
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

	// Termino de GLFW
	glfwTerminate();

	return 0;
}

//----------animación----------
/*
	Incremento de las posiciones y llamado a calculo de interpolaciones 
*/
void animacion() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) { //end of animation between frames?
			playIndex++;
			if (playIndex > FrameIndex - 2)	{//end of total animation?
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else { //Next frame interpolations
				i_curr_steps = 0; //Reset counter
				interpolation(); //Interpolation
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			i_curr_steps++;
		}

	}
}

//----------DoMovement----------
/*
	Controles de camara
	Alteración de la camara basado en entradas
	Controles de animaciones
*/

void DoMovement() {
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) 
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) 
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) 
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) 
		camera.ProcessKeyboard(RIGHT, deltaTime);

	//Incrementos para animación sencilla 1
	if ((limite) && (rot <= 0)) 
		rot += 0.5;
	
	if ((!limite) && (rot >= -90)) 
		rot -= 0.5;

	//Incrementos para animación sencilla 2

	if (activarLampara == true) 
		rotLamp = sin(glfwGetTime() * 0.5f) * 25.0;
	
	if (activarLampara == false) 
		rotLamp = 0 ;

	//Incrementos para animación sencilla 3

	//Cajon izquierdo
	if ((cajIzq) && (tCajIzq >= -0.8)) 
		tCajIzq -= 0.01;
	
	if ((!cajIzq) && (tCajIzq <= 0.0)) 
		tCajIzq += 0.01;
	
	//Cajon central
	if ((cajCen) && (tCajCen >= -0.8)) 
		tCajCen -= 0.01;
	
	if ((!cajCen ) && (tCajCen <= 0.0)) 
		tCajCen += 0.01;
	
	//Cajon derecho
	if ((cajDer) && (tCajDer >= -0.8)) 
		tCajDer -= 0.01;
	
	if ((!cajDer) && (tCajDer <= 0.0)) 
		tCajDer += 0.01;
	
	//Cajon silla
	if ((sill) && (tSilla >= -0.8)) 
		tSilla -= 0.01;
	
	if ((!sill) && (tSilla <= 0.0)) 
		tSilla += 0.01;
	
	//Incrementos para animación compleja 1

	if (animate) {
		if (coordU < 1.0f) {
			Sleep(150);
			coordU += 0.25f;
		}
		else {
			coordU = 0.0f;
			coordV -= 0.25;
			if (coordU < -1.0)
				coordV = 0.0f;
		}
	}
	else {
		coordU = 0.0f;
		coordV = 0.0f;
	}
}

//----------KeyCallback----------
/*
	Controles de animaciones
	Ejecucuón mas lenta que DoMovement
*/

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	//Control de animación sencilla 1
	
	//Activador
	if (keys[GLFW_KEY_1] && rot <= -90) 
		limite = true;
	
	if (keys[GLFW_KEY_1] && rot >= 0) 
		limite = false;

	//Control de animación sencilla 2
	
	//valores de Spotlights
	if (keys[GLFW_KEY_2]) {
		if (active == false ) {
			encenderPointLight = glm::vec3(0.05, 0.05, 0.05);
			encenderSpotLight = glm::vec3(0.0, 0.0, 0.0);
			rotSpotLight = 0.0;
			active = true;
		}else {	
			encenderPointLight = glm::vec3(0.0 , 0.0, 0.0);
			encenderSpotLight = glm::vec3(1.0, 1.0, 1.0);
			rotSpotLight = 0.25;
			active = false;
		}
	}

	//Activador
	if (keys[GLFW_KEY_2]) {
		activarLampara = !activarLampara;
	}

	//Control de animación sencilla 3

	//Cajon izquierdo
	//Activador
	if (keys[GLFW_KEY_3] && tCajIzq <= -0.8) {
		cajIzq = false;
	}
	if (keys[GLFW_KEY_3] && tCajIzq >= 0) {
		cajIzq = true;
	}

	//Cajon central
	//Activador
	if (keys[GLFW_KEY_4] && tCajCen <= -0.8) 
		cajCen = false;

	if (keys[GLFW_KEY_4] && tCajCen >= 0) 
		cajCen = true;
	
	//Cajon derecho
	//Activador
	if (keys[GLFW_KEY_5] && tCajDer <= -0.8) 
		cajDer = false;
	
	if (keys[GLFW_KEY_5] && tCajDer >= 0) 
		cajDer = true;
	
	//Silla
	//Activador
	if (keys[GLFW_KEY_6] && tSilla <= -0.8)
		sill = false;

	if (keys[GLFW_KEY_6] && tSilla >= 0)
		sill = true;

	//Control de animación compleja 1

	//Activador
	if (keys[GLFW_KEY_7]) 
		animate = !animate;
	
	//Control de animación compleja 2

	//Activador
	if (keys[GLFW_KEY_8]) {
		if (cargaDeDatos) {
			tiroParabolico();
			cargaDeDatos = false;
		}
		if (play == false && (FrameIndex > 1)) {
			resetElements();
			interpolation();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else {
			play = false;
		}
	}	
}

//----------KeyCallback----------
/*
	Reconocimiento del movimiento del mouse
*/

void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
	if (firstMouse)	{
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