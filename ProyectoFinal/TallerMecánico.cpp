#include <iostream>
#include <cmath>
#include <vector>
#include <string>

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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
float MoveToward(float current, float target, float maxStep);

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 1.6f, 7.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Iluminación
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 3.6f, -5.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f)
};

bool interiorLightOn = true;
bool flashlightOn = false;

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

// Animación de las puertas
const glm::vec3 SLIDE_DOOR_POS(-1.8f, 0.0f, 0.345f);
const glm::vec3 HINGE_A_POS(0.3f, 0.0f, 0.345f);
const glm::vec3 HINGE_B_POS(3.3f, 0.0f, 0.345f);

const float SLIDE_MAX = 3.0f;
const float SLIDE_SPEED = 1.0f;
const float SWING_MAX = 90.0f;  // apertura máxima (grados)
const float SWING_SPEED = 60.0f;

bool slideOpen = false;
bool swingAOpen = false;
bool swingBOpen = false;

float slideOffset = 0.0f;
float swingAngleA = 0.0f;
float swingAngleB = 0.0f;

// Gato hidráulico de piso + automóvil
const glm::vec3 CAR_POS(1.8f, 0.0f, -5.0f);
const float CAR_SCALE = 1.0f;
const float CAR_YAW = 0.0f;
const float CAR_Y_OFFSET = 0.0f;
const float CAR_CONTACT_Y = 0.215f;
const glm::vec2 CAR_JACK_XZ(0.72f, -0.5f);

const glm::vec3 STREET_POS(-1.62f, -0.63f, 2.22f);
const float STREET_YAW = -90.0f;

// Árbol en la banqueta; las hojas se dibujan con transparencia
const glm::vec3 TREE_POS(-6.3f, 0.0f, 1.4f);

// Perro sentado afuera de las puertas batientes
const glm::vec3 DOG_POS(2.0f, 0.0f, 1.05f);
const float DOG_YAW = 0.0f;

// Calle y banquetas: la banqueta se repite en tramos de 28 m; la de enfrente es la misma girada 180°
const float STREET_WIDTH = 10.0f;  // ancho de la calle (m)
const float FAR_SIDEWALK_Z = 2.4f + STREET_WIDTH + 2.4f;  // borde exterior de la banqueta de enfrente
const float SIDEWALK_LEN = 28.0f;
// Casas: origen al centro del frente, a ras de piso, frente hacia +Z
const float CASA_FRONT_Z = 0.2f;
const glm::vec3 CASA_POS[5] = {
	glm::vec3(-14.3f, -0.1f, CASA_FRONT_Z),  // izquierda del taller
	glm::vec3(10.4f, -0.1f, CASA_FRONT_Z),  // derecha del taller
	glm::vec3(-16.03f, -0.1f, FAR_SIDEWALK_Z + 0.2f),  // enfrente, izquierda
	glm::vec3(0.0f, -0.1f, FAR_SIDEWALK_Z + 0.2f),  // enfrente, centro
	glm::vec3(16.03f, -0.1f, FAR_SIDEWALK_Z + 0.2f)  // enfrente, derecha
};
const float CASA_YAW[5] = { 0.0f, 0.0f, 180.0f, 180.0f, 180.0f };

// Autos en la calle: máquina de estados (1 = avanzando, 2 = reinicia al llegar al final)
const float CAR_RANGE = 75.0f;  // recorrido en x (m)
const float LANE_NEAR_Z = 4.9f;  // carril junto al taller
const float LANE_FAR_Z = 9.9f;  // carril de enfrente
const float PRIUS_SPEED = 7.0f;  // m/s
const float JEEP_SPEED = 5.5f;  // m/s
const float CAR_Y = -0.15f;  // altura sobre el asfalto

const float EXT_SCALE = 1.3f;
const glm::vec3 EXT_POS[2] = { glm::vec3(-3.88f, 0.90f, -2.95f), glm::vec3(3.88f, 0.90f, -1.50f) };
const float EXT_YAW[2] = { 90.0f, -90.0f };

// Rueda recargada en la pared izquierda
const glm::vec3 WHEEL_POS(-3.835f, 0.339f, -8.40f);
const float WHEEL_TILT = 12.0f;

// Elevador de 2 columnas a la izquierda del taller
const glm::vec3 LIFT_POS(-1.95f, 0.0f, -5.5f);

// Mercedes sobre el elevador (girado 180°)
const glm::vec3 MERC_POS(-1.95f, 0.06f, -5.405f);
const float MERC_YAW = 180.0f;

// Mesa negra contra la pared del fondo, con la caja de herramientas encima
const glm::vec3 TABLE_POS(0.30f, 0.0f, -9.73f);
const float TABLE_YAW = 0.0f;
const glm::vec3 TOOLBOX_POS(0.15f, 0.872f, -9.80f);
const float TOOLBOX_YAW = 0.0f;

// Gabinete rodante junto a la mesa
const glm::vec3 CABINET_POS(-0.69f, 0.0f, -9.775f);
const float CABINET_YAW = 90.0f;

// Herramientas sobre la mesa y el gabinete (origen en el centro de la base)
struct TOOL_PLACE { glm::vec3 pos; float yaw; };
const TOOL_PLACE HAMMER_P = { glm::vec3(0.15f, 0.872f, -9.575f), 90.0f };  // mesa
const TOOL_PLACE SPANNER_P = { glm::vec3(0.60f, 0.872f, -9.78f), 0.0f };  // mesa
const TOOL_PLACE TAPE_P = { glm::vec3(0.67f, 0.872f, -9.56f), 0.0f };  // mesa
const TOOL_PLACE NUT_P[3] = { { glm::vec3(0.47f, 0.872f, -9.57f), 0.0f }, { glm::vec3(0.50f, 0.872f, -9.61f), 40.0f },
                              { glm::vec3(-0.42f, 0.933f, -9.72f), 15.0f } };  // mesa y gabinete
const TOOL_PLACE SCREW_P[2] = { { glm::vec3(0.44f, 0.872f, -9.53f), 30.0f }, { glm::vec3(-0.98f, 0.929f, -9.80f), 20.0f } };
const TOOL_PLACE WRENCH2_P = { glm::vec3(-0.55f, 0.927f, -9.84f), 90.0f };  // gabinete
const TOOL_PLACE SCREWDRIVER_P = { glm::vec3(-0.85f, 0.935f, -9.70f), 90.0f };  // gabinete

// Llave de cruz recargada junto a la rueda
const glm::vec3 WRENCH_POS(-3.953f, 0.245f, -9.30f);
const float WRENCH_LEAN = 12.0f;

// Ventilador de pedestal; las aspas son una pieza aparte que gira
const glm::vec3 FAN_POS(3.30f, 0.0f, -8.90f);
const float FAN_YAW = -35.0f;
const glm::vec3 FAN_HUB(0.0025f, 0.8072f, 0.2044f);  // posición del cubo
const float FAN_SPEED = 200.0f;  // grados por segundo
float prisX = -CAR_RANGE, jeepX = CAR_RANGE;  // posición en x de cada auto
int prisAnim = 1, jeepAnim = 1;  // estado de cada auto
float fanAngle = 0.0f;  // ángulo de las aspas

// Trabajadores: uno en la mesa y otro junto al sedán
const glm::vec3 WORKER1_POS(0.55f, 0.0f, -8.95f);
const float WORKER1_YAW = 180.0f;
const glm::vec3 WORKER2_POS(0.20f, 0.0f, -4.60f);
const float WORKER2_YAW = 90.0f;

// Mecánico (piernas, torso y cabeza); pivotes en cintura y cuello
const glm::vec3 MECH_POS(-1.95f, 0.0f, -8.5f);
const float MECH_YAW = 0.0f;
const glm::vec3 MECH_WAIST(0.0f, 0.7447f, 0.0f);
const glm::vec3 MECH_NECK_OFF(0.0f, 0.7521f, 0.0f);   // cuello respecto a la cintura

const float JACK_L = 0.40f;
const glm::vec3 ARM_PIVOT(0.0f, 0.10f, 0.18f);
const glm::vec3 HANDLE_PIVOT(0.0f, 0.16f, 0.30f);
const float PAD_H = 0.05f;
const float ARM_MIN = glm::degrees(asinf(glm::clamp((CAR_CONTACT_Y - 0.10f - PAD_H) / JACK_L, 0.0f, 0.9f)));
const float ARM_MAX = 58.0f;
const float HANDLE_MIN = 20.0f, HANDLE_MAX = 70.0f;
const float PUMP_RATIO = 0.14f;
const float HANDLE_SPEED = 90.0f;
const float ARM_SPEED = 25.0f;

glm::vec3 JACK_POS = CAR_POS + glm::vec3(CAR_JACK_XZ.x + (JACK_L * cosf(glm::radians(ARM_MIN)) - ARM_PIVOT.z), 0.0f, CAR_JACK_XZ.y);

float handleAng = HANDLE_MAX;
float armAng = ARM_MIN;

// KeyFrames
#define MAX_FRAMES 32
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame {
	float handle;
	float handleInc;
	float arm;
	float armInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

void resetElements(void)
{
	handleAng = KeyFrame[0].handle;
	armAng = KeyFrame[0].arm;
}

void interpolation(void)
{
	KeyFrame[playIndex].handleInc = (KeyFrame[playIndex + 1].handle - KeyFrame[playIndex].handle) / i_max_steps;
	KeyFrame[playIndex].armInc = (KeyFrame[playIndex + 1].arm - KeyFrame[playIndex].arm) / i_max_steps;
}

// ---- Animación del mecánico ----
// mLean: inclinación del cuerpo [grados]
// mRoll: ladeo de la cabeza [grados]
// mYaw: giro de la cabeza [grados]
float mLean = 0.0f, mRoll = 0.0f, mYaw = 0.0f;

#define M_MAX_FRAMES 16
int m_max_steps = 40;
int m_curr_steps = 0;
typedef struct _mframe {
	float lean, leanInc;
	float roll, rollInc;
	float yaw, yawInc;
}MFRAME;

MFRAME MKey[M_MAX_FRAMES];
int MFrameIndex = 0;
bool mPlay = false;
int mPlayIndex = 0;

void mResetElements(void)
{
	mLean = MKey[0].lean;
	mRoll = MKey[0].roll;
	mYaw = MKey[0].yaw;
}

void mInterpolation(void)
{
	MKey[mPlayIndex].leanInc = (MKey[mPlayIndex + 1].lean - MKey[mPlayIndex].lean) / m_max_steps;
	MKey[mPlayIndex].rollInc = (MKey[mPlayIndex + 1].roll - MKey[mPlayIndex].roll) / m_max_steps;
	MKey[mPlayIndex].yawInc = (MKey[mPlayIndex + 1].yaw - MKey[mPlayIndex].yaw) / m_max_steps;
}

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final - Taller Impala", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skydomeShader("Shader/skydome.vs", "Shader/skydome.frag");  // Shader del skydome

	Model Taller((char*)"Models/Taller/Taller_Impala.obj");
	Model PuertaCorrediza((char*)"Models/Taller/Puerta_corrediza_izq.obj");
	Model PuertaBatienteA((char*)"Models/Taller/Puerta_batiente_A.obj");
	Model PuertaBatienteB((char*)"Models/Taller/Puerta_batiente_B.obj");
	Model GatoCuerpo((char*)"Models/Gato/Gato_cuerpo.obj");
	Model GatoPalanca((char*)"Models/Gato/Gato_palanca.obj");
	Model GatoBrazo((char*)"Models/Gato/Gato_brazo.obj");
	Model Auto((char*)"Models/Sedan/Sedan.obj");
	Model Calle((char*)"Models/Objetos/Road.obj");
	Model Suelo((char*)"Models/Objetos/Suelo.obj");
	Model Extintor((char*)"Models/Objetos/Extintor.obj");
	Model Rueda((char*)"Models/Objetos/Rueda.obj");
	Model Elevador((char*)"Models/Objetos/Elevador.obj");
	Model Mercedes((char*)"Models/Objetos/Mercedes_W222.obj");
	Model Mesa((char*)"Models/Objetos/Mesa_negra.obj");
	Model CajaHerr((char*)"Models/Objetos/Caja_herramientas.obj");
	Model LlaveCruz((char*)"Models/Objetos/Llave_cruz.obj");
	Model MecPiernas((char*)"Models/Objetos/Mecanico_piernas.obj");
	Model MecTorso((char*)"Models/Objetos/Mecanico_torso.obj");
	Model VentCuerpo((char*)"Models/Objetos/Ventilador_cuerpo.obj");
	Model VentAspas((char*)"Models/Objetos/Ventilador_aspas.obj");
	Model TrabajadorNormal((char*)"Models/Objetos/Obrero_normal.obj");
	Model TrabajadorGordo((char*)"Models/Objetos/Obrero_gordo.obj");
	Model ArbolTronco((char*)"Models/Objetos/Arbol_tronco.obj");
	Model ArbolHojas((char*)"Models/Objetos/Arbol_hojas.obj");
	Model Perro((char*)"Models/Objetos/Perro.obj");
	Model Gabinete((char*)"Models/Objetos/Gabinete.obj");
	Model Martillo((char*)"Models/Objetos/Herr_martillo.obj");
	Model Destornillador((char*)"Models/Objetos/Herr_destornillador.obj");
	Model LlaveEspanola((char*)"Models/Objetos/Herr_llave_espanola.obj");
	Model LlaveCombinada((char*)"Models/Objetos/Herr_llave_combinada.obj");
	Model Tuerca((char*)"Models/Objetos/Herr_tuerca.obj");
	Model Tornillo((char*)"Models/Objetos/Herr_tornillo.obj");
	Model Cinta((char*)"Models/Objetos/Herr_cinta.obj");
	Model MecCabeza((char*)"Models/Objetos/Mecanico_cabeza.obj");
	Model Casa1((char*)"Models/Objetos/Casa1.obj");
	Model Casa2((char*)"Models/Objetos/Casa2.obj");
	Model Casa3((char*)"Models/Objetos/Casa3.obj");
	Model Casa4((char*)"Models/Objetos/Casa4.obj");
	Model Casa5((char*)"Models/Objetos/Casa5.obj");
	Model Prius((char*)"Models/Objetos/Prius.obj");
	Model Jeep((char*)"Models/Objetos/Jeep.obj");
	Model* Casas[5] = { &Casa4, &Casa1, &Casa3, &Casa2, &Casa5 }; // mismo orden que CASA_POS
	Model Cielo((char*)"Models/Skybox/SM_Skybox.FBX");  // Skydome

	GLuint skyTex = 0;
	{
		int skyW = 0, skyH = 0;
		unsigned char* skyImg = SOIL_load_image("Models/Skybox/textures/skybox_skydays_3.png", &skyW, &skyH, 0, SOIL_LOAD_RGB);
		if (skyImg == nullptr)
		{
			std::cout << "ERROR: no se pudo cargar Models/Skybox/textures/skybox_skydays_3.png" << std::endl;
		}
		else
		{
			glGenTextures(1, &skyTex);
			glBindTexture(GL_TEXTURE_2D, skyTex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, skyW, skyH, 0, GL_RGB, GL_UNSIGNED_BYTE, skyImg);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // evita costuras
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			SOIL_free_image_data(skyImg);
		}
	}

	// Inicializar Keyframes
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].handle = 0;
		KeyFrame[i].handleInc = 0;
		KeyFrame[i].arm = 0;
		KeyFrame[i].armInc = 0;
	}

	KeyFrame[0].handle = 70.0000f;
	KeyFrame[0].arm = 12.3736f;
	KeyFrame[1].handle = 20.0000f;
	KeyFrame[1].arm = 19.3736f;
	KeyFrame[2].handle = 70.0000f;
	KeyFrame[2].arm = 19.5348f;
	KeyFrame[3].handle = 20.0000f;
	KeyFrame[3].arm = 26.5348f;
	KeyFrame[4].handle = 70.0000f;
	KeyFrame[4].arm = 26.5348f;
	KeyFrame[5].handle = 20.0000f;
	KeyFrame[5].arm = 33.5348f;
	KeyFrame[6].handle = 70.0000f;
	KeyFrame[6].arm = 33.5348f;
	KeyFrame[7].handle = 20.0000f;
	KeyFrame[7].arm = 40.5348f;
	KeyFrame[8].handle = 70.0000f;
	KeyFrame[8].arm = 40.5348f;
	KeyFrame[9].handle = 70.0000f;
	KeyFrame[9].arm = 9.3520f;
	FrameIndex = 10;

	resetElements();
	interpolation();
	playIndex = 0;
	i_curr_steps = 0;
	play = false;

	// Keyframes del mecánico
	for (int i = 0; i < M_MAX_FRAMES; i++) { MKey[i].lean = MKey[i].roll = MKey[i].yaw = 0.0f; MKey[i].leanInc = MKey[i].rollInc = MKey[i].yawInc = 0.0f; }
	MKey[0].lean = 34.76f; MKey[0].roll = 0.00f; MKey[0].yaw = 0.00f;
	MKey[1].lean = 34.76f; MKey[1].roll = 27.33f; MKey[1].yaw = 0.00f;
	MKey[2].lean = 34.76f; MKey[2].roll = -35.00f; MKey[2].yaw = 0.00f;
	MKey[3].lean = 5.11f; MKey[3].roll = 2.37f; MKey[3].yaw = 0.00f;
	MKey[4].lean = 5.11f; MKey[4].roll = 2.37f; MKey[4].yaw = 37.65f;
	MKey[5].lean = 5.11f; MKey[5].roll = 2.37f; MKey[5].yaw = -32.52f;
	MKey[6].lean = 5.11f; MKey[6].roll = 2.37f; MKey[6].yaw = 33.26f;
	MKey[7].lean = 5.11f; MKey[7].roll = 2.37f; MKey[7].yaw = -31.58f;
	MKey[8].lean = 5.11f; MKey[8].roll = 2.37f; MKey[8].yaw = 2.73f;
	MKey[9].lean = 34.76f; MKey[9].roll = 0.00f; MKey[9].yaw = 0.00f;
	MFrameIndex = 10;
	mResetElements();
	mPlayIndex = 0;
	m_curr_steps = 0;
	mInterpolation();
	mPlay = false;


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 0);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 400.0f);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();
		Animation();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		lightingShader.Use();

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Variables de Iluminación
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.45f, 0.45f, 0.45f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);

		glm::vec3 pointAmbient = interiorLightOn ? glm::vec3(0.20f, 0.19f, 0.17f) : glm::vec3(0.0f);
		glm::vec3 pointDiffuse = interiorLightOn ? glm::vec3(0.95f, 0.90f, 0.80f) : glm::vec3(0.0f);
		glm::vec3 pointSpecular = interiorLightOn ? glm::vec3(0.5f, 0.5f, 0.5f) : glm::vec3(0.0f);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), pointAmbient.x, pointAmbient.y, pointAmbient.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), pointDiffuse.x, pointDiffuse.y, pointDiffuse.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), pointSpecular.x, pointSpecular.y, pointSpecular.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		glm::vec3 spotColor = flashlightOn ? glm::vec3(0.8f, 0.8f, 0.7f) : glm::vec3(0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), spotColor.x * 0.2f, spotColor.y * 0.2f, spotColor.z * 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), spotColor.x, spotColor.y, spotColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		glm::mat4 view = camera.GetViewMatrix();
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Renderizado de modelos
		glm::mat4 model(1);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Taller.Draw(lightingShader);

		// Suelo de asfalto
		Suelo.Draw(lightingShader);

		// Banquetas del taller y de enfrente
		for (int s = -1; s <= 1; s++)
		{
			model = glm::translate(glm::mat4(1), glm::vec3(SIDEWALK_LEN * s, 0.0f, 0.0f));
			model = glm::translate(model, STREET_POS);
			model = glm::rotate(model, glm::radians(STREET_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Calle.Draw(lightingShader);

			model = glm::translate(glm::mat4(1), glm::vec3(SIDEWALK_LEN * s, 0.0f, FAR_SIDEWALK_Z));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, STREET_POS);
			model = glm::rotate(model, glm::radians(STREET_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Calle.Draw(lightingShader);
		}

		// Casas
		for (int i = 0; i < 5; i++)
		{
			model = glm::translate(glm::mat4(1), CASA_POS[i]);
			model = glm::rotate(model, glm::radians(CASA_YAW[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Casas[i]->Draw(lightingShader);
		}

		for (int i = 0; i < 2; i++)
		{
			model = glm::translate(glm::mat4(1), EXT_POS[i]);
			model = glm::rotate(model, glm::radians(EXT_YAW[i]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(EXT_SCALE));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Extintor.Draw(lightingShader);
		}

		// Rueda recargada en la pared izquierda
		model = glm::translate(glm::mat4(1), WHEEL_POS);
		model = glm::rotate(model, glm::radians(WHEEL_TILT), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Rueda.Draw(lightingShader);

		// Elevador de autos
		model = glm::translate(glm::mat4(1), LIFT_POS);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Elevador.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, SLIDE_DOOR_POS + glm::vec3(-slideOffset, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PuertaCorrediza.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, HINGE_A_POS);
		model = glm::rotate(model, glm::radians(swingAngleA), glm::vec3(0.0f, 1.0f, 0.0f));  // abre hacia adentro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PuertaBatienteA.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, HINGE_B_POS);
		model = glm::rotate(model, glm::radians(-swingAngleB), glm::vec3(0.0f, 1.0f, 0.0f));  // abre hacia adentro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PuertaBatienteB.Draw(lightingShader);

		float armRad = glm::radians(armAng);
		float armMinRad = glm::radians(ARM_MIN);
		float roll = JACK_L * (cosf(armMinRad) - cosf(armRad));

		glm::mat4 jackBase = glm::translate(glm::mat4(1), JACK_POS);
		jackBase = glm::rotate(jackBase, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 bodyMat = glm::translate(jackBase, glm::vec3(0.0f, 0.0f, -roll));
		glm::mat4 modelTemp = bodyMat;

		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoCuerpo.Draw(lightingShader);

		model = glm::translate(modelTemp, HANDLE_PIVOT);
		model = glm::rotate(model, glm::radians(-handleAng), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GatoPalanca.Draw(lightingShader);

		glm::mat4 armMat = glm::translate(modelTemp, ARM_PIVOT);
		armMat = glm::rotate(armMat, armRad, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(armMat));
		GatoBrazo.Draw(lightingShader);

		glm::vec4 tip = armMat * glm::vec4(0.0f, 0.0f, -JACK_L, 1.0f);
		float padTop = tip.y + PAD_H;
		float lift = glm::max(0.0f, padTop - CAR_CONTACT_Y);

		glm::mat4 carBase = glm::translate(glm::mat4(1), CAR_POS + glm::vec3(0.0f, CAR_Y_OFFSET, 0.0f));
		carBase = glm::rotate(carBase, glm::radians(CAR_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		carBase = glm::scale(carBase, glm::vec3(CAR_SCALE));
		model = glm::translate(glm::mat4(1), glm::vec3(0.0f, lift, 0.0f)) * carBase;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Auto.Draw(lightingShader);

		// Mercedes
		model = glm::translate(glm::mat4(1), MERC_POS);
		model = glm::rotate(model, glm::radians(MERC_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mercedes.Draw(lightingShader);

		// Mesa negra, caja de herramientas encima y llave de cruz al lado
		model = glm::translate(glm::mat4(1), TABLE_POS);
		model = glm::rotate(model, glm::radians(TABLE_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.Draw(lightingShader);

		model = glm::translate(glm::mat4(1), TOOLBOX_POS);
		model = glm::rotate(model, glm::radians(TOOLBOX_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CajaHerr.Draw(lightingShader);

		// Gabinete rodante junto a la mesa
		model = glm::translate(glm::mat4(1), CABINET_POS);
		model = glm::rotate(model, glm::radians(CABINET_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Gabinete.Draw(lightingShader);

		// Herramientas sobre la mesa y sobre el gabinete
		auto drawTool = [&](Model& m, const TOOL_PLACE& tp)
		{
			glm::mat4 mt = glm::translate(glm::mat4(1), tp.pos);
			mt = glm::rotate(mt, glm::radians(tp.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mt));
			m.Draw(lightingShader);
		};
		drawTool(Martillo, HAMMER_P);
		drawTool(LlaveEspanola, SPANNER_P);
		drawTool(Cinta, TAPE_P);
		for (int i = 0; i < 3; i++) drawTool(Tuerca, NUT_P[i]);
		for (int i = 0; i < 2; i++) drawTool(Tornillo, SCREW_P[i]);
		drawTool(LlaveCombinada, WRENCH2_P);
		drawTool(Destornillador, SCREWDRIVER_P);

		// Llave de cruz recargada
		model = glm::translate(glm::mat4(1), WRENCH_POS);
		model = glm::rotate(model, glm::radians(WRENCH_LEAN - 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LlaveCruz.Draw(lightingShader);

		// Ventilador: cuerpo fijo y aspas giratorias
		glm::mat4 fanBase = glm::translate(glm::mat4(1), FAN_POS);
		fanBase = glm::rotate(fanBase, glm::radians(FAN_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(fanBase));
		VentCuerpo.Draw(lightingShader);

		glm::mat4 bladesMat = glm::translate(fanBase, FAN_HUB);
		bladesMat = glm::rotate(bladesMat, glm::radians(-fanAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(bladesMat));
		VentAspas.Draw(lightingShader);

		// Árbol: tronco y hojas con transparencia
		model = glm::translate(glm::mat4(1), TREE_POS);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ArbolTronco.Draw(lightingShader);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		ArbolHojas.Draw(lightingShader);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// Perro
		model = glm::translate(glm::mat4(1), DOG_POS);
		model = glm::rotate(model, glm::radians(DOG_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Perro.Draw(lightingShader);

		// Trabajadores
		model = glm::translate(glm::mat4(1), WORKER1_POS);
		model = glm::rotate(model, glm::radians(WORKER1_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TrabajadorNormal.Draw(lightingShader);

		model = glm::translate(glm::mat4(1), WORKER2_POS);
		model = glm::rotate(model, glm::radians(WORKER2_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TrabajadorGordo.Draw(lightingShader);

		// Mecanico jerarquico: piernas (base) -> torso (cintura) -> cabeza (cuello)
		glm::mat4 mechBase = glm::translate(glm::mat4(1), MECH_POS);
		mechBase = glm::rotate(mechBase, glm::radians(MECH_YAW), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mechBase));
		MecPiernas.Draw(lightingShader);

		glm::mat4 torsoMat = glm::translate(mechBase, MECH_WAIST);
		torsoMat = glm::rotate(torsoMat, glm::radians(mLean), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(torsoMat));
		MecTorso.Draw(lightingShader);

		glm::mat4 headMat = glm::translate(torsoMat, MECH_NECK_OFF);
		headMat = glm::rotate(headMat, glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		headMat = glm::rotate(headMat, glm::radians(mRoll), glm::vec3(0.0f, 0.0f, 1.0f));
		headMat = glm::rotate(headMat, glm::radians(0.3f * fabsf(mRoll)), glm::vec3(1.0f, 0.0f, 0.0f));  // cabeceo ligero
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(headMat));
		MecCabeza.Draw(lightingShader);

		// Autos en la calle
		model = glm::translate(glm::mat4(1), glm::vec3(prisX, CAR_Y, LANE_FAR_Z));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));   // frente hacia +X
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Prius.Draw(lightingShader);

		model = glm::translate(glm::mat4(1), glm::vec3(jeepX, CAR_Y, LANE_NEAR_Z));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // frente hacia -X
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Jeep.Draw(lightingShader);

		// --- Renderizado del Skydome FBX ---
		glDepthFunc(GL_LEQUAL);
		skydomeShader.Use();

		// Quitar la traslación de la vista
		glm::mat4 viewSky = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skydomeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewSky));
		glUniformMatrix4fv(glGetUniformLocation(skydomeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 modelSky = glm::mat4(1.0f);
		modelSky = glm::scale(modelSky, glm::vec3(10.0f)); // escala del cielo
		modelSky = glm::rotate(modelSky, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // orientación del FBX
		glUniformMatrix4fv(glGetUniformLocation(skydomeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSky));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skyTex);
		glUniform1i(glGetUniformLocation(skydomeShader.Program, "texture_diffuse1"), 0);
		Cielo.Draw(skydomeShader);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_1: slideOpen = !slideOpen; break;
		case GLFW_KEY_2: swingAOpen = !swingAOpen; break;
		case GLFW_KEY_3: swingBOpen = !swingBOpen; break;
		case GLFW_KEY_4:
		{
			bool abrir = !(slideOpen && swingAOpen && swingBOpen);
			slideOpen = swingAOpen = swingBOpen = abrir;
			break;
		}
		case GLFW_KEY_SPACE: interiorLightOn = !interiorLightOn; break;
		case GLFW_KEY_F: flashlightOn = !flashlightOn; break;
		case GLFW_KEY_G:
			if (play) play = false;
			else if (FrameIndex > 1) play = true;
			break;
		case GLFW_KEY_M:
			if (mPlay) mPlay = false;
			else if (MFrameIndex > 1)
			{
				mResetElements();
				mPlayIndex = 0;
				m_curr_steps = 0;
				mInterpolation();
				mPlay = true;
			}
			break;
		}
	}
}

float MoveToward(float current, float target, float maxStep)
{
	if (current < target)
		return glm::min(current + maxStep, target);
	if (current > target)
		return glm::max(current - maxStep, target);
	return current;
}

void Animation()
{
	slideOffset = MoveToward(slideOffset, slideOpen ? SLIDE_MAX : 0.0f, SLIDE_SPEED * deltaTime);
	swingAngleA = MoveToward(swingAngleA, swingAOpen ? SWING_MAX : 0.0f, SWING_SPEED * deltaTime);
	swingAngleB = MoveToward(swingAngleB, swingBOpen ? SWING_MAX : 0.0f, SWING_SPEED * deltaTime);

	// Ventilador
	// Autos
	if (prisAnim == 1)
	{
		prisX += PRIUS_SPEED * deltaTime;
		if (prisX > CAR_RANGE) prisAnim = 2;
	}
	if (prisAnim == 2) { prisX = -CAR_RANGE; prisAnim = 1; }
	if (jeepAnim == 1)
	{
		jeepX -= JEEP_SPEED * deltaTime;
		if (jeepX < -CAR_RANGE) jeepAnim = 2;
	}
	if (jeepAnim == 2) { jeepX = CAR_RANGE; jeepAnim = 1; }

	fanAngle += FAN_SPEED * deltaTime;
	if (fanAngle >= 360.0f) fanAngle -= 360.0f;

	if (play)
	{
		if (i_curr_steps >= i_max_steps)
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)
			{
				play = false;
				playIndex = 0;
				i_curr_steps = 0;
				resetElements();
				interpolation();
			}
			else
			{
				i_curr_steps = 0;
				interpolation();
			}
		}
		else
		{
			handleAng += KeyFrame[playIndex].handleInc;
			armAng += KeyFrame[playIndex].armInc;
			i_curr_steps++;
		}
	}

	if (mPlay)
	{
		if (m_curr_steps >= m_max_steps)
		{
			mPlayIndex++;
			if (mPlayIndex > MFrameIndex - 2)
			{
				mPlay = false;
				mPlayIndex = 0;
				m_curr_steps = 0;
				mResetElements();
				mInterpolation();
			}
			else
			{
				m_curr_steps = 0;
				mInterpolation();
			}
		}
		else
		{
			mLean += MKey[mPlayIndex].leanInc;
			mRoll += MKey[mPlayIndex].rollInc;
			mYaw += MKey[mPlayIndex].yawInc;
			m_curr_steps++;
		}
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
	GLfloat yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}