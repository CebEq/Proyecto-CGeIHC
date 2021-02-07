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
#include "Texture.h"
#include "modelAnim.h"
#include "meshAnim.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animation();

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 1.6f, -5.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
GLfloat fast = 1.0f;
bool fastFlag = false;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(8.63309f, 4.03836f, 96.6376f),
	glm::vec3(8.63309f,  4.03836f, 106.364f),
	glm::vec3(-8.65143,  4.03836f, 96.6359f),
	glm::vec3(-8.65143f,  4.03836f,  106.364f)
};

glm::vec3 LightP1;

// Camera handling
bool freeCam = true;
bool staticCam = false;

// Hanging lamps animation
float lampsRot = 0.0f;
bool lampsAnim = false;

// Sofas animation
float sophasRot = 0.0f;
float sophasTempTran = 3.167f / 2.0f;
bool sophasAnimOpen = false;
bool sophasAnimClose = false;

// Tablet animation
float tabletPosX = 0.0f;
float tabletScale = 1.0f;
bool tabletAnimStart = false;
bool tabletAnimEnd = false;

// Trolley animation
float trolleyMovX = 0.0;
float trolleyMovZ = 0.0;

bool trolleyCircuit = false;
bool trolleyCircuit1 = true;
bool trolleyCircuit2 = false;
bool trolleyCircuit3 = false;
bool trolleyCircuit4 = false;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
glm::vec3 inPosCandle(0.863392f, 0.54336f, 95.7726f);
glm::vec3 inRotCandle(0.0f, 0.0f, 0.0f);
float posX = inPosCandle.x, posY = inPosCandle.y, posZ = inPosCandle.z;
float rotX = inRotCandle.x, rotY = inRotCandle.y, rotZ = inRotCandle.z;

#define MAX_FRAMES 4
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		// X position variable
	float posY;		// Y position variable
	float posZ;		// Z position variable
	float rotX;		// X rotation variable
	float rotY;		// Y rotation variable
	float rotZ;		// Z rotation variable
	float posIncX;	// X position increment variable
	float posIncY;	// Y position increment variable
	float posIncZ;	// Z position increment variable
	float rotIncX;	// X rotation increment variable
	float rotIncY;	// Y rotation increment variable
	float rotIncZ;	// Z rotation increment variable
	int steps;
}FRAME;

FRAME KeyFrameCandle[MAX_FRAMES];
int FrameIndexCandle = 0;		// Starting frame
bool playCandle = false;
int playIndexCandle = 0;


glm::vec3 inPosSyringe(0.956524f, 1.425150, 87.661003f);
glm::vec3 inRotSyringe(75.000000f, 0.0f, 16.000000f);
float SposX = inPosSyringe.x, SposY = inPosSyringe.y, SposZ = inPosSyringe.z;
float SrotX = inRotSyringe.x, SrotY = inRotSyringe.y, SrotZ = inRotSyringe.z;

glm::vec3 inPosSyringeP(1.056524f, 1.454260, 87.661003f);
glm::vec3 inRotSyringeP(75.000000f, 0.0f, 16.000000f);
float SPposX = inPosSyringeP.x, SPposY = inPosSyringeP.y, SPposZ = inPosSyringeP.z;
float SProtX = inRotSyringeP.x, SProtY = inRotSyringeP.y, SProtZ = inRotSyringeP.z;

#define MAX_FRAMES_S 2
int i_max_steps_s = 190;
int i_curr_steps_s = 0;

FRAME KeyFrameS[MAX_FRAMES_S];
int FrameIndexS = 0;		// Starting frame
bool playS = false;
int playIndexS = 0;

#define MAX_FRAMES_SP 4
int i_max_steps_sp = 190;
int i_curr_steps_sp = 0;

FRAME KeyFrameSP[MAX_FRAMES_SP];
int FrameIndexSP = 0;		// Starting frame
bool playSP = false;
int playIndexSP = 0;

void initialiseKeyframes(FRAME KeyFrame[], int type)
{
	if (type == 1)
	{
		printf("Keyframes for candle\n");

		FrameIndexCandle = 0;

		KeyFrame[FrameIndexCandle].posX = posX;
		KeyFrame[FrameIndexCandle].posY = posY;
		KeyFrame[FrameIndexCandle].posZ = posZ;

		KeyFrame[FrameIndexCandle].rotX = rotX;
		KeyFrame[FrameIndexCandle].rotY = rotY;
		KeyFrame[FrameIndexCandle].rotZ = rotZ;

		KeyFrame[FrameIndexCandle].steps = 10;

		printf("Frameindex %d\t", FrameIndexCandle);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", posX, posY, posZ, rotX, rotY, rotZ);

		FrameIndexCandle = 1;

		KeyFrame[FrameIndexCandle].posX = 1.043391;
		KeyFrame[FrameIndexCandle].posY = 0.583360;
		KeyFrame[FrameIndexCandle].posZ = 95.623306;

		KeyFrame[FrameIndexCandle].rotX = -35.000000;
		KeyFrame[FrameIndexCandle].rotY = 0.000000;
		KeyFrame[FrameIndexCandle].rotZ = -53.000000;

		KeyFrame[FrameIndexCandle].steps = 10;

		printf("Frameindex %d\t", FrameIndexCandle);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", KeyFrame[FrameIndexCandle].posX, KeyFrame[FrameIndexCandle].posY, KeyFrame[FrameIndexCandle].posZ, KeyFrame[FrameIndexCandle].rotX, KeyFrame[FrameIndexCandle].rotY, KeyFrame[FrameIndexCandle].rotZ);

		FrameIndexCandle = 2;

		KeyFrame[FrameIndexCandle].posX = 1.163390;
		KeyFrame[FrameIndexCandle].posY = 0.093360;
		KeyFrame[FrameIndexCandle].posZ = 95.474731;

		KeyFrame[FrameIndexCandle].rotX = -44.999969;
		KeyFrame[FrameIndexCandle].rotY = 0.000000;
		KeyFrame[FrameIndexCandle].rotZ = -86.999931;

		KeyFrame[FrameIndexCandle].steps = 20;

		printf("FrameIndexCandle %d\t", FrameIndexCandle);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", KeyFrame[FrameIndexCandle].posX, KeyFrame[FrameIndexCandle].posY, KeyFrame[FrameIndexCandle].posZ, KeyFrame[FrameIndexCandle].rotX, KeyFrame[FrameIndexCandle].rotY, KeyFrame[FrameIndexCandle].rotZ);

		FrameIndexCandle = 3;

		KeyFrame[FrameIndexCandle].posX = 0.813394;
		KeyFrame[FrameIndexCandle].posY = 0.093360;
		KeyFrame[FrameIndexCandle].posZ = 95.375046;

		KeyFrame[FrameIndexCandle].rotX = -140.000076;
		KeyFrame[FrameIndexCandle].rotY = -90.000000;
		KeyFrame[FrameIndexCandle].rotZ = -91.000031;

		KeyFrame[FrameIndexCandle].steps = 20;

		printf("FrameIndexCandle %d\t", FrameIndexCandle);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", KeyFrame[FrameIndexCandle].posX, KeyFrame[FrameIndexCandle].posY, KeyFrame[FrameIndexCandle].posZ, KeyFrame[FrameIndexCandle].rotX, KeyFrame[FrameIndexCandle].rotY, KeyFrame[FrameIndexCandle].rotZ);

		FrameIndexCandle = 4;
	}
	else if (type == 2)
	{
		printf("\nKeyframes for syringe base\n");

		FrameIndexS = 0;

		KeyFrame[FrameIndexS].posX = SposX;
		KeyFrame[FrameIndexS].posY = SposY;
		KeyFrame[FrameIndexS].posZ = SposZ;

		KeyFrame[FrameIndexS].rotX = SrotX;
		KeyFrame[FrameIndexS].rotY = SrotY;
		KeyFrame[FrameIndexS].rotZ = SrotZ;

		KeyFrame[FrameIndexS].steps = 25;

		printf("FrameIndexS %d\t", FrameIndexS);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", SposX, SposY, SposZ, SrotX, SrotY, SrotZ);

		FrameIndexS = 1;

		KeyFrame[FrameIndexS].posX = 0.856524;
		KeyFrame[FrameIndexS].posY = 1.405150;
		KeyFrame[FrameIndexS].posZ = SposZ;

		KeyFrame[FrameIndexS].rotX = SrotX;
		KeyFrame[FrameIndexS].rotY = SrotY;
		KeyFrame[FrameIndexS].rotZ = SrotZ;

		KeyFrame[FrameIndexS].steps = 20;

		printf("FrameIndexS %d\t", FrameIndexS);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", KeyFrame[FrameIndexS].posX, KeyFrame[FrameIndexS].posY, KeyFrame[FrameIndexS].posZ, KeyFrame[FrameIndexS].rotX, KeyFrame[FrameIndexS].rotY, KeyFrame[FrameIndexS].rotZ);

		FrameIndexS = 2;
	}
	else if (type == 3)
	{
		printf("\nKeyframes for syringe plunger\n");

		FrameIndexSP = 0;

		KeyFrame[FrameIndexSP].posX = SPposX;
		KeyFrame[FrameIndexSP].posY = SPposY;
		KeyFrame[FrameIndexSP].posZ = SPposZ;

		KeyFrame[FrameIndexSP].rotX = SProtX;
		KeyFrame[FrameIndexSP].rotY = SProtY;
		KeyFrame[FrameIndexSP].rotZ = SProtZ;

		KeyFrame[FrameIndexSP].steps = 25;

		printf("FrameIndexSP %d\t", FrameIndexSP);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", SPposX, SPposY, SPposZ, SProtX, SProtY, SProtZ);

		FrameIndexSP = 1;

		KeyFrame[FrameIndexSP].posX = 0.956524;
		KeyFrame[FrameIndexSP].posY = 1.433150;
		KeyFrame[FrameIndexSP].posZ = SPposZ;

		KeyFrame[FrameIndexSP].rotX = SProtX;
		KeyFrame[FrameIndexSP].rotY = SProtY;
		KeyFrame[FrameIndexSP].rotZ = SProtZ;

		KeyFrame[FrameIndexSP].steps = 30;

		printf("FrameIndexS %d\t", FrameIndexSP);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", KeyFrame[FrameIndexSP].posX, KeyFrame[FrameIndexSP].posY, KeyFrame[FrameIndexSP].posZ, KeyFrame[FrameIndexSP].rotX, KeyFrame[FrameIndexSP].rotY, KeyFrame[FrameIndexSP].rotZ);

		FrameIndexSP = 2;

		KeyFrame[FrameIndexSP].posX = 0.876524;
		KeyFrame[FrameIndexSP].posY = 1.410550;
		KeyFrame[FrameIndexSP].posZ = SposZ;

		KeyFrame[FrameIndexSP].rotX = SrotX;
		KeyFrame[FrameIndexSP].rotY = SrotY;
		KeyFrame[FrameIndexSP].rotZ = SrotZ;

		KeyFrame[FrameIndexSP].steps = 30;

		printf("FrameIndexS %d\t", FrameIndexSP);
		printf("posX: %f, posY: %f, posZ: %f, rotX: %f, rotY: %f, rotZ: %f\n", KeyFrame[FrameIndexSP].posX, KeyFrame[FrameIndexSP].posY, KeyFrame[FrameIndexSP].posZ, KeyFrame[FrameIndexSP].rotX, KeyFrame[FrameIndexSP].rotY, KeyFrame[FrameIndexSP].rotZ);

		FrameIndexSP = 3;
	}
}

void resetElements(FRAME KeyFrame[], int type)
{
	if (type == 1)
	{
		posX = KeyFrame[0].posX;
		posY = KeyFrame[0].posY;
		posZ = KeyFrame[0].posZ;

		rotX = KeyFrame[0].rotX;
		rotY = KeyFrame[0].rotY;
		rotZ = KeyFrame[0].rotZ;
	}
	else if (type == 2)
	{
		SposX = KeyFrame[0].posX;
		SposY = KeyFrame[0].posY;
		SposZ = KeyFrame[0].posZ;

		SrotX = KeyFrame[0].rotX;
		SrotY = KeyFrame[0].rotY;
		SrotZ = KeyFrame[0].rotZ;
	}
	else if (type == 3)
	{
		SPposX = KeyFrame[0].posX;
		SPposY = KeyFrame[0].posY;
		SPposZ = KeyFrame[0].posZ;

		SProtX = KeyFrame[0].rotX;
		SProtY = KeyFrame[0].rotY;
		SProtZ = KeyFrame[0].rotZ;
	}
}

void interpolation(FRAME KeyFrame[], int playIndex)
{
	KeyFrame[playIndex].posIncX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / KeyFrame[playIndex].steps;
	KeyFrame[playIndex].posIncY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / KeyFrame[playIndex].steps;
	KeyFrame[playIndex].posIncZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / KeyFrame[playIndex].steps;
	KeyFrame[playIndex].rotIncX = (KeyFrame[playIndex + 1].rotX - KeyFrame[playIndex].rotX) / KeyFrame[playIndex].steps;
	KeyFrame[playIndex].rotIncY = (KeyFrame[playIndex + 1].rotY - KeyFrame[playIndex].rotY) / KeyFrame[playIndex].steps;
	KeyFrame[playIndex].rotIncZ = (KeyFrame[playIndex + 1].rotZ - KeyFrame[playIndex].rotZ) / KeyFrame[playIndex].steps;
}

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto - CNCE", nullptr, nullptr);

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
	printf("%f\n", glfwGetTime());

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

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");


	 // Load models
	Model facade((char*)"Models/Facade/Facade.obj");
	Model table((char*)"Models/Table/Table.obj");
	Model sofa((char*)"Models/Sofa/Sofa.obj");
	Model fountain((char*)"Models/Fountain/Fountain.obj");
	Model bell((char*)"Models/Bell/Bell.obj");
	Model candle((char*)"Models/Candle/Candle.obj");
	Model coffee((char*)"Models/Coffee/Coffee.obj");
	Model hLamp((char*)"Models/HangingLamp/HLamp.obj");
	Model lift((char*)"Models/Lift/Lift.obj");
	Model liquor((char*)"Models/Liquor/Liquor.obj");
	Model plant((char*)"Models/Plant/Plant.obj");
	Model trolley((char*)"Models/Trolley/Trolley.obj");
	Model smCandle((char*)"Models/SmallCandle/SmallCandle.obj");
	Model pillowBench((char*)"Models/PillowBench/PillowBench.obj");
	Model tablet((char*)"Models/Tablet/Tablet.obj");
	Model trashcan((char*)"Models/Trashcan/Trashcan.obj");
	Model signFloor((char*)"Models/SignFloor/SignFloor.obj");
	Model signWall((char*)"Models/SignWall/SignWall.obj");
	Model syringe((char*)"Models/Syringe/Syringe_Base.obj");
	Model syringePlunger((char*)"Models/Syringe/Syringe_Plunger.obj");

	ModelAnim femaleNurse("AnimatedModels/FemaleNurse.fbx");
	femaleNurse.initShaders(animShader.Program);

	ModelAnim maleNurse("AnimatedModels/MaleNurse.fbx");
	maleNurse.initShaders(animShader.Program);

	ModelAnim patient("AnimatedModels/Patient.fbx");
	patient.initShaders(animShader.Program);

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		(250.0f / 256.0f), (0.0f / 256.0f), //Back
		 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,		(160.0f / 256.0f), (0.0f / 256.0f),
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		(160.0f / 256.0f), (230.0f / 256.0f),
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		(160.0f / 256.0f), (230.0f / 256.0f),
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		(250.0f / 256.0f), (230.0f / 256.0f),
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		(250.0f / 256.0f), (0.0f / 256.0f), //Back

		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		(160.0f / 256.0f), (0.0f / 256.0f), //Front
		 0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		(250.0f / 256.0f), (0.0f / 256.0f),
		 0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		(250.0f / 256.0f), (230.0f / 256.0f),
		 0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		(250.0f / 256.0f), (230.0f / 256.0f),
		-0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		(160.0f / 256.0f), (230.0f / 256.0f),
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		(160.0f / 256.0f), (0.0f / 256.0f), //Front

		// Positions            // Normals              // Texture Coords
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (0.0f / 256.0f), //Right
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (0.0f / 256.0f),
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (230.0f / 256.0f),
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (230.0f / 256.0f),
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (230.0f / 256.0f),
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (0.0f / 256.0f), //Right

		 // Positions            // Normals              // Texture Coords
		-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (230.0f / 256.0f), //Left
		-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (230.0f / 256.0f),
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (0.0f / 256.0f),
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (0.0f / 256.0f),
		-0.5f, -0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (0.0f / 256.0f),
		-0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (230.0f / 256.0f), //Left

		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		(0.0f / 256.0f), (60.0f / 256.0f), //Bottom
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		(60.0f / 256.0f), (60.0f / 256.0f),
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		(60.0f / 256.0f), (120.0f / 256.0f),
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		(60.0f / 256.0f), (120.0f / 256.0f),
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		(0.0f / 256.0f), (120.0f / 256.0f),
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		(0.0f / 256.0f), (60.0f / 256.0f), //Bottom

		// Positions            // Normals              // Texture Coords
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		(0.0f / 256.0f), (60.0f / 256.0f), //Top
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		(60.0f / 256.0f), (60.0f / 256.0f),
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		(60.0f / 256.0f), (120.0f / 256.0f),
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		(60.0f / 256.0f), (120.0f / 256.0f),
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		(0.0f / 256.0f), (120.0f / 256.0f),
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		(0.0f / 256.0f), (60.0f / 256.0f), //Top


	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/sh_rt.png");
	faces.push_back("SkyBox/sh_lf.png");
	faces.push_back("SkyBox/sh_up.png");
	faces.push_back("SkyBox/sh_dn.png");
	faces.push_back("SkyBox/sh_bk.png");
	faces.push_back("SkyBox/sh_ft.png");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	// Load textures
	GLuint lampTex, lampTex2;
	glGenTextures(1, &lampTex);
	glGenTextures(1, &lampTex2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lampTex);
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("Textures/TableLamp_Mat-Base.png", &textureWidth, &textureHeight, &nrChannels, 0);
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lampTex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("Textures/TableLamp_Mat-Base2.png", &textureWidth, &textureHeight, &nrChannels, 0);
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

	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	lightingShader.Use();

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// List of transparent objects and their positions
	// Last number of each vector denotates the type
	// 1 for hanging lamp (1.1 on the left, 1.2 on the right)
	// 2 for liquor
	// 3 for small candle
	vector<glm::vec4> transparentObj
	{
		glm::vec4(8.63309f, 3.41569f, 106.364f, 1.2f),
		glm::vec4(-8.65143f, 3.41569f, 106.364f, 1.1f),
		glm::vec4(8.63309f, 3.41569f, 96.6376f, 1.2f),
		glm::vec4(-8.65143f, 3.41569f, 96.6376f, 1.1f),
		glm::vec4(-9.18576f, 0.527963f, 96.5703f, 2.0f),
		glm::vec4(9.43814f, 0.530196f, 106.914f, 3.0f),
		glm::vec4(-9.29368f, 0.521843f, 106.258f, 3.0f),
		glm::vec4(9.71299f, 0.530196f, 97.3501f, 3.0f),
		glm::vec4(9.28725f, 0.523051f, 96.9009f, 3.0f),
		glm::vec4(-9.44402f, 0.52826f, 96.6139f, 3.0f),
		glm::vec4(-9.36705f, 0.531023f, 96.4145f, 3.0f)
	};

	initialiseKeyframes(KeyFrameCandle, 1);
	initialiseKeyframes(KeyFrameS, 2);
	initialiseKeyframes(KeyFrameSP, 3);

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
		animation();

		if (!fastFlag)
		{
			fast = 1.0f;
		}

		if (staticCam)
		{
			camera.SetPosition(glm::vec3(0.679385f, 1.43f, 88.1023f));
			camera.SetDirection(-180.0f, 0.0f);
		}

		// Sort transparent objects according to distance to camera
		std::map<float, glm::vec4> sorted;
		for (unsigned int i = 0; i < transparentObj.size(); i++)
		{
			glm::vec3 objPos(transparentObj[i]);
			float distance = glm::length(camera.GetPosition() - objPos);
			sorted[distance] = transparentObj[i];
		}

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		lightingShader.setVec3("lightPos", lightPos);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.4f, -0.7f, -0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.611f, 0.603f, 0.588f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.980f, 0.913f, 0.780f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.980f, 0.913f, 0.780f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.14f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.07f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.980f, 0.913f, 0.780f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.980f, 0.913f, 0.780f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.14f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.07f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.980f, 0.913f, 0.780f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.980f, 0.913f, 0.780f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.14f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.07f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.980f, 0.913f, 0.780f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.980f, 0.913f, 0.780f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.14f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.07f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 10.0f);

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


		// Facade
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		facade.Draw(lightingShader);

		// Tables
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.18434f, 0.0f, 97.032f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.72379f, 0.0f, 97.032f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.18434f, 0.0f, 106.577f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.72379f, 0.0f, 106.577f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.01554f, 0.0f, 96.7657f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-9.47992f, 0.0f, 96.7657f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.83178f, 0.0f, 106.312f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-9.29502f, 0.0f, 106.312f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);

		// Fountain
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.06918f, 0.0f, 98.0488f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		fountain.Draw(lightingShader);

		// Sofas
		// Animated - Simple Animation (Rotation)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.56549f, 0.0f, 94.6254f));
		model = glm::translate(model, glm::vec3(sophasTempTran, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-sophasRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-sophasTempTran, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.9765f, 0.0f, 96.9653f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.56549f, 0.0f, 99.1645f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.56549f, 0.0f, 104.174f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.9763f, 0.0f, 106.511f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.56549f, 0.0f, 108.712f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		// Animated - Simple Animation (Rotation)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.86277f, 0.0f, 94.6267f));
		model = glm::translate(model, glm::vec3(-sophasTempTran, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(sophasRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(sophasTempTran, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-11.2708f, 0.0f, 96.8323f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.85851f, 0.0f, 99.1716f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.67636f, 0.0f, 104.171f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-11.0889f, 0.0f, 106.374f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.67516f, 0.0f, 108.718f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sofa.Draw(lightingShader);

		// Trolley
		// Animated - Complex Animation (Equation)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.94712f, 0.0f, 109.09f));
		model = glm::translate(model, glm::vec3(trolleyMovX, 0.0f, trolleyMovZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		trolley.Draw(lightingShader);

		// Table Lamp
		// Top
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lampTex);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.10024f, 0.852f, 97.4154f));
		model = glm::scale(model, glm::vec3(0.248f, 0.485f, 0.248f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);
		//Bottom
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lampTex2);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.10024f, 0.596f, 97.4154f));
		model = glm::scale(model, glm::vec3(0.191f, 0.028f, 0.191f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.10024f, 0.556f, 97.4154f));
		model = glm::scale(model, glm::vec3(0.248f, 0.051f, 0.248f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);

		// Top
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lampTex);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.10131f, 0.852f, 106.973f));
		model = glm::scale(model, glm::vec3(0.248f, 0.485f, 0.248f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);
		//Bottom
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lampTex2);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.10131f, 0.596f, 106.973f));
		model = glm::scale(model, glm::vec3(0.191f, 0.028f, 0.191f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.10131f, 0.556f, 106.973f));
		model = glm::scale(model, glm::vec3(0.248f, 0.051f, 0.248f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);

		// Top
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lampTex);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.4106f, 0.852f, 96.3411f));
		model = glm::scale(model, glm::vec3(0.248f, 0.485f, 0.248f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);
		//Bottom
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lampTex2);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.4106f, 0.596f, 96.3411f));
		model = glm::scale(model, glm::vec3(0.191f, 0.028f, 0.191f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.4106f, 0.556f, 96.3411f));
		model = glm::scale(model, glm::vec3(0.248f, 0.051f, 0.248f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);

		// Top
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lampTex);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.21907f, 0.852f, 105.912f));
		model = glm::scale(model, glm::vec3(0.248f, 0.485f, 0.248f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);
		//Bottom
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, lampTex2);
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.21907f, 0.596f, 105.912f));
		model = glm::scale(model, glm::vec3(0.191f, 0.028f, 0.191f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.21907f, 0.556f, 105.912f));
		model = glm::scale(model, glm::vec3(0.248f, 0.051f, 0.248f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);

		// Bells
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.36703f, 0.531f, 97.46f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bell.Draw(lightingShader);

		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.829748f, 0.543438f, 100.331f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bell.Draw(lightingShader);

		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.10591f, 0.531f, 96.6893f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bell.Draw(lightingShader);

		glBindVertexArray(VAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.80377f, 0.531f, 106.29f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bell.Draw(lightingShader);

		// Candles
		// Animated - Complex Animation (Keyframes)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		candle.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.088279f, 0.991933f, 109.304f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		candle.Draw(lightingShader);

		// Coffee
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.61247f, 0.520745f, 106.355f));
		model = glm::rotate(model, glm::radians(28.3233f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		coffee.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.37096f, 0.520745f, 106.319f));
		model = glm::rotate(model, glm::radians(-149.732f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		coffee.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.64638f, 0.520745f, 96.3043f));
		model = glm::rotate(model, glm::radians(-59.7354f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		coffee.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.67166f, 0.520745f, 97.0427f));
		model = glm::rotate(model, glm::radians(-217.384f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		coffee.Draw(lightingShader);

		// Lifts
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.47919f, 0.0f, 109.893f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lift.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.72292f, 0.0f, 109.893f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lift.Draw(lightingShader);

		// Plants
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.7994f, 0.0f, 108.502f));
		model = glm::rotate(model, glm::radians(-52.5289f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plant.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.1163f, 0.0f, 104.3f));
		model = glm::rotate(model, glm::radians(-26.4095f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plant.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.1163f, 0.0f, 99.3561f));
		model = glm::rotate(model, glm::radians(-26.4095f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plant.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.1163f, 0.0f, 94.756f));
		model = glm::rotate(model, glm::radians(-26.4095f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plant.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-11.0615f, 0.0f, 108.515f));
		model = glm::rotate(model, glm::radians(163.449f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plant.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-11.2971f, 0.0f, 103.989f));
		model = glm::rotate(model, glm::radians(153.756f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plant.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-11.2971f, 0.0f, 99.0367f));
		model = glm::rotate(model, glm::radians(153.756f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plant.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-11.2971f, 0.0f, 94.4495f));
		model = glm::rotate(model, glm::radians(153.756f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plant.Draw(lightingShader);

		// Pillow Bench
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.160291f, 0.0f, 87.5972f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pillowBench.Draw(lightingShader);

		// Tablet
		// Animated - Simple Animation (Scalation and Translation)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-9.41696f, 0.531508f, 105.869f));
		model = glm::translate(model, glm::vec3(tabletPosX, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(329.951f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(tabletScale, 1.0f, tabletScale));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tablet.Draw(lightingShader);

		// Trashcan
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.093992f, 0.0f, 109.3080f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		trashcan.Draw(lightingShader);

		// Floor Signs
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 64.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		signFloor.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 64.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		signFloor.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 54.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		signFloor.Draw(lightingShader);

		// Wall Signs
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.154983f, 3.50899f, 87.2863f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.929782f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		signWall.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-11.8565f, 3.03645f, 96.952f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.20931f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		signWall.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.6774f, 3.03645f, 96.952f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.20931f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		signWall.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 3.88585f, 109.717f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.844937f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		signWall.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 3.88585f, 80.7157f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.844937f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		signWall.Draw(lightingShader);

		// Draw transparent objects from furthest to nearest
		for (std::map<float, glm::vec4>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			glm::vec3 objPos(it->second);
			model = glm::translate(model, objPos);
			if (it->second.w == 1.1f)
			{
				// Animated - Simple Animation (Rotation)
				model = glm::rotate(model, lampsRot, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				hLamp.Draw(lightingShader);
			}
			else if (it->second.w == 1.2f)
			{
				// Animated - Simple Animation (Rotation)
				model = glm::rotate(model, -lampsRot, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				hLamp.Draw(lightingShader);
			}
			else if (it->second.w == 2)
			{
				model = glm::rotate(model, glm::radians(30.2387f), glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				liquor.Draw(lightingShader);
			}
			else
			{
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				smCandle.Draw(lightingShader);
			}
		}

		if (staticCam)
		{
			// Syringe Plunge
			// Animated - Complex Animation (Keyframes)
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(SPposX, SPposY, SPposZ));
			model = glm::rotate(model, glm::radians(SProtZ), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(SProtY), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(SProtX), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.45f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			syringePlunger.Draw(lightingShader);

			// Syringe
			// Animated - Complex Animation (Keyframes)
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(SposX, SposY, SposZ));
			model = glm::rotate(model, glm::radians(SrotZ), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(SrotY), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(SrotX), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.56214f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			syringe.Draw(lightingShader);
		}

		glBindVertexArray(0);

		/*_______________________________Personaje Animado___________________________*/

		animShader.Use();

		glUniform1f(glGetUniformLocation(animShader.Program, "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.ambient"), 0.611f, 0.603f, 0.588f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.diffuse"), 0.611f, 0.603f, 0.588f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.specular"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.direction"), -0.4f, -0.7f, -0.8f);

		view = camera.GetViewMatrix();

		modelLoc = glGetUniformLocation(animShader.Program, "model");
		viewLoc = glGetUniformLocation(animShader.Program, "view");
		projLoc = glGetUniformLocation(animShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.7f, 0.0f, 94.6f));
		model = glm::scale(model, glm::vec3(0.0135f));	// it's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		femaleNurse.Draw(animShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.7f, 0.0f, 96.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0135f));	// it's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		maleNurse.Draw(animShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.329385f, 0.05f, 87.5523f));
		model = glm::scale(model, glm::vec3(0.0155f));	// it's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		patient.Draw(animShader);

		glBindVertexArray(0);

		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

void animation()
{
	// Trolley animation

	if (trolleyCircuit)
	{
		if (trolleyCircuit1)
		{
			trolleyMovZ -= 0.1f;
			if (trolleyMovZ < -19.9f)
			{
				trolleyCircuit1 = false;
				trolleyCircuit2 = true;
			}
		}

		if (trolleyCircuit2)
		{
			trolleyMovX -= 0.1f;
			if (trolleyMovX < -11.9f)
			{
				trolleyCircuit2 = false;
				trolleyCircuit3 = true;
			}
		}

		if (trolleyCircuit3)
		{
			trolleyMovZ += 0.1f;
			if (trolleyMovZ > -1.9f)
			{
				trolleyCircuit3 = false;
				trolleyCircuit4 = true;
			}
		}

		if (trolleyCircuit4)
		{
			trolleyMovX += 0.1f;
			if (trolleyMovX > -0.1f)
			{
				trolleyCircuit4 = false;
				trolleyCircuit1 = true;
			}
		}
	}

	// Candle animation

	if (playCandle)
	{
		if (i_curr_steps >= KeyFrameCandle[playIndexCandle].steps) //end of animation between frames?
		{
			playIndexCandle++;
			if (playIndexCandle > FrameIndexCandle - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndexCandle = 0;
				playCandle = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter								  
				interpolation(KeyFrameCandle, playIndexCandle);  //Interpolation
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrameCandle[playIndexCandle].posIncX;
			posY += KeyFrameCandle[playIndexCandle].posIncY;
			posZ += KeyFrameCandle[playIndexCandle].posIncZ;
			rotX += KeyFrameCandle[playIndexCandle].rotIncX;
			rotY += KeyFrameCandle[playIndexCandle].rotIncY;
			rotZ += KeyFrameCandle[playIndexCandle].rotIncZ;

			i_curr_steps++;
		}
	}

	// Hanging lamps animation

	if (lampsAnim)
	{
		lampsRot = (float)glfwGetTime();
	}

	// Sophas animation

	if (sophasAnimOpen)
	{
		if (sophasRot < 60.0f)
		{
			sophasRot += 1.0f;
		}
		else
		{
			sophasAnimOpen = false;
		}
	}
	else if(sophasAnimClose)
	{
		if (sophasRot > 0.0f)
		{
			sophasRot -= 1.0f;
		}
		else
		{
			sophasAnimClose = false;
		}
	}

	// Tablet animation

	if (tabletAnimStart)
	{
		if (tabletPosX < 0.5f)
		{
			tabletPosX += 0.05f;
			tabletScale += 0.05f;
		}
		else
		{
			tabletAnimStart = false;
		}
	}
	else if (tabletAnimEnd)
	{
		if (tabletPosX > 0.05f)
		{
			tabletPosX -= 0.05f;
			tabletScale -= 0.05f;
		}
		else
		{
			tabletPosX = 0.0f;
			tabletScale = 1.0f;
			tabletAnimEnd = false;
		}
	}

	// Syringe base animation

	if (playS)
	{
		if (i_curr_steps_s >= KeyFrameS[playIndexS].steps) //end of animation between frames?
		{
			playIndexS++;
			if (playIndexS > FrameIndexS - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndexS = 0;
				playS = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps_s = 0; //Reset counter								  
				interpolation(KeyFrameS, playIndexS);  //Interpolation
			}
		}
		else
		{
			//Draw animation
			SposX += KeyFrameS[playIndexS].posIncX;
			SposY += KeyFrameS[playIndexS].posIncY;
			SposZ += KeyFrameS[playIndexS].posIncZ;
			SrotX += KeyFrameS[playIndexS].rotIncX;
			SrotY += KeyFrameS[playIndexS].rotIncY;
			SrotZ += KeyFrameS[playIndexS].rotIncZ;

			i_curr_steps_s++;
		}
	}

	// Syringe plunger animation

	if (playSP)
	{
		if (i_curr_steps_sp >= KeyFrameSP[playIndexSP].steps) //end of animation between frames?
		{
			playIndexSP++;
			if (playIndexSP > FrameIndexSP - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndexSP = 0;
				playSP = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps_sp = 0; //Reset counter								  
				interpolation(KeyFrameSP, playIndexSP);  //Interpolation
			}
		}
		else
		{
			//Draw animation
			SPposX += KeyFrameSP[playIndexSP].posIncX;
			SPposY += KeyFrameSP[playIndexSP].posIncY;
			SPposZ += KeyFrameSP[playIndexSP].posIncZ;
			SProtX += KeyFrameSP[playIndexSP].rotIncX;
			SProtY += KeyFrameSP[playIndexSP].rotIncY;
			SProtZ += KeyFrameSP[playIndexSP].rotIncZ;

			i_curr_steps_sp++;
		}
	}
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	if (keys[GLFW_KEY_LEFT_SHIFT])
	{
		fast = 3.0f;
		fastFlag = true;
	}
	else
	{
		fastFlag = false;
	}

	// Camera controls
	if ((keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) && freeCam )
	{
		camera.ProcessKeyboard(FORWARD, deltaTime, fast);
	}

	if ((keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) && freeCam)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime, fast);
	}

	if ((keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) && freeCam)
	{
		camera.ProcessKeyboard(LEFT, deltaTime, fast);
	}

	if ((keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) && freeCam)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime, fast);
	}

	if ((keys[GLFW_KEY_Q] || keys[GLFW_KEY_RIGHT]) && freeCam)
	{
		camera.ProcessKeyboard(UP, deltaTime, fast);
	}

	if ((keys[GLFW_KEY_E] || keys[GLFW_KEY_RIGHT]) && freeCam)
	{
		camera.ProcessKeyboard(DOWN, deltaTime, fast);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_M])
	{
		freeCam = true;
		staticCam = false;

		playS = false;
		SposX = inPosSyringe.x;
		SposY = inPosSyringe.y;
		SposZ = inPosSyringe.z;
		SrotX = inRotSyringe.x;
		SrotY = inRotSyringe.y;
		SrotZ = inRotSyringe.z;

		playSP = false;
		SPposX = inPosSyringeP.x;
		SPposY = inPosSyringeP.y;
		SPposZ = inPosSyringeP.z;
		SProtX = inRotSyringeP.x;
		SProtY = inRotSyringeP.y;
		SProtZ = inRotSyringeP.z;
	}

	if (keys[GLFW_KEY_N])
	{
		freeCam = false;
		staticCam = true;
	}

	if (keys[GLFW_KEY_V] && staticCam)
	{
		if (playS == false && (FrameIndexS > 1) && playSP == false && (FrameIndexSP > 1))
		{
			resetElements(KeyFrameS, 2);
			//First Interpolation				
			interpolation(KeyFrameS, playIndexS);

			playS = true;
			playIndexS = 0;
			i_curr_steps_s = 0;

			resetElements(KeyFrameSP, 3);
			//First Interpolation				
			interpolation(KeyFrameSP, playIndexSP);

			playSP = true;
			playIndexSP = 0;
			i_curr_steps_sp = 0;
		}
		else
		{
			playS = false;
			playSP = false;
		}
	}

	if (keys[GLFW_KEY_B] && staticCam)
	{
		playS = false;
		SposX = inPosSyringe.x;
		SposY = inPosSyringe.y;
		SposZ = inPosSyringe.z;
		SrotX = inRotSyringe.x;
		SrotY = inRotSyringe.y;
		SrotZ = inRotSyringe.z;

		playSP = false;
		SPposX = inPosSyringeP.x;
		SPposY = inPosSyringeP.y;
		SPposZ = inPosSyringeP.z;
		SProtX = inRotSyringeP.x;
		SProtY = inRotSyringeP.y;
		SProtZ = inRotSyringeP.z;
	}

	if (keys[GLFW_KEY_1])
	{
		trolleyCircuit = !trolleyCircuit;
	}

	if (keys[GLFW_KEY_2])
	{
		trolleyCircuit = false;
		trolleyMovX = 0.0f;
		trolleyMovZ = 0.0f;
	}

	if (keys[GLFW_KEY_3])
	{
		if (playCandle == false && (FrameIndexCandle > 1))
		{
			resetElements(KeyFrameCandle, 1);
			//First Interpolation				
			interpolation(KeyFrameCandle, playIndexCandle);

			playCandle = true;
			playIndexCandle = 0;
			i_curr_steps = 0;
		}
		else
		{
			playCandle = false;
		}
	}

	if (keys[GLFW_KEY_4])
	{
		playCandle = false;
		posX = inPosCandle.x;
		posY = inPosCandle.y;
		posZ = inPosCandle.z;
		rotX = inRotCandle.x;
		rotY = inRotCandle.y;
		rotZ = inRotCandle.z;
	}

	if (keys[GLFW_KEY_5])
	{
		lampsAnim = true;
	}

	if (keys[GLFW_KEY_6])
	{
		lampsAnim = false;
		lampsRot = 0.0f;
	}

	if (keys[GLFW_KEY_7])
	{
		if (!sophasAnimOpen && !sophasAnimClose)
		{
			if (sophasRot == 0.0f)
			{
				sophasAnimOpen = true;
				sophasAnimClose = false;
			}
			else
			{
				sophasAnimOpen = false;
				sophasAnimClose = true;
			}			
		}
		else if (sophasAnimOpen && !sophasAnimClose)
		{
			sophasAnimOpen = false;
			sophasAnimClose = true;
		}
		else if (!sophasAnimOpen && sophasAnimClose)
		{
			sophasAnimOpen = true;
			sophasAnimClose = false;
		}
		else
		{
			sophasAnimOpen = false;
			sophasAnimClose = false;
		}
	}

	if (keys[GLFW_KEY_8])
	{
		sophasAnimOpen = false;
		sophasAnimClose = false;
		sophasRot = 0.0f;
	}

	if (keys[GLFW_KEY_9])
	{
		if (!tabletAnimStart && !tabletAnimEnd)
		{
			if (tabletPosX == 0.00f)
			{
				tabletAnimStart = true;
				tabletAnimEnd = false;
			}
			else
			{
				tabletAnimStart = false;
				tabletAnimEnd = true;
			}
		}
		else if (tabletAnimStart && !tabletAnimEnd)
		{
			tabletAnimStart = false;
			tabletAnimEnd = true;
		}
		else if (!tabletAnimStart && tabletAnimEnd)
		{
			tabletAnimStart = true;
			tabletAnimEnd = false;
		}
		else
		{
			tabletAnimStart = false;
			tabletAnimEnd = false;
		}
	}

	if (keys[GLFW_KEY_0])
	{
		tabletAnimStart = false;
		tabletAnimEnd = false;
		tabletPosX = 0.00f;
		tabletScale = 1.00f;
	}

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

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 1.0f, 1.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
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

