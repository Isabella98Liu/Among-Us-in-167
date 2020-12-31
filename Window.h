#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "Material.h"

#include "Node.h"
#include "Transform.h"
#include "Geometry.h"
#include "Character.h"
#include "Physics.h"
#include "ParticleSystem.h"

#include "CommonValues.h"

#include "irrKlang.h"
#include "stb_image.h"

using namespace irrklang;

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
	static bool keys[1024];
	static glm::vec2 last_cursor_pos;
	static glm::vec2 cursor_pos;
	static GLfloat xChange;
	static GLfloat yChange;
	static GLfloat deltaTime;
	static GLfloat  lastTime;

	// Camera Matrices
	static Camera* camera;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos;

	// Shader Program ID
	static GLuint objectShader;
	static GLuint toonShadingShader;
	static GLuint particleSystemShader;

	// Sound effect
	static ISoundEngine* SoundEngine;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	static glm::vec3 trackBallMapping(glm::vec2 start);

	static GLuint loadTexture(std::string fileName);

	static void initializeColor();
	static void initializePlayer();
	static void initializeEnvironmentCollision();
	static void addEnvironmentCollision(Character* character);

	static void nonPlayerControl(GLfloat deltaTime);
	static void nonPlayerMovement(GLfloat deltaTime);

	static void particleSystemControl(GLfloat deltaTime);

	static void generateCharacter(int type, glm::vec3 color);
	static void reuseCharacter(int index);
	static void disableCharacter(int index);
	static void deleteCharacter(int index);

	static glm::vec3 getColor();
	static void recycleColor(glm::vec3 color);

	bool* getKeys() { return keys; }
	static GLfloat getRandFloat(int min, int max) { return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))); }
	static glm::vec2 getRandPoint(int x_min, int x_max, int y_min, int y_max) { return glm::vec2(getRandFloat(x_min, x_max), getRandFloat(y_min, y_max)); }

};

#endif
