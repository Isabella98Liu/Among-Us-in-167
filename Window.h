#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
	static glm::vec2 cursor_pos;
	static glm::vec2 last_cursor_pos;
	static bool is_rotating;

	// Objects to Render
	static Model* bear;
	static Model* bunny;
	static Model* sandal;

	// Light objects
	static DirectionalLight* directionalLight;
	static PointLight* pointLight;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;

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

	static glm::vec3 ballMapping(glm::vec2 start);
};

#endif
