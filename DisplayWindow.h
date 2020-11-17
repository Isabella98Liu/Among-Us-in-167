#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "BaseObject.h"
#include "Cube.h"
#include "PointCloud.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SkyBox.h"

#include "CommonValues.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

class DisplayWindow
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
	static glm::vec2 cursor_pos;
	static glm::vec2 last_cursor_pos;

	// Objects to Render
	static Model* bear;
	static Model* bunny;
	static Model* sandal;

	// Light objects
	static DirectionalLight* directionalLight;
	static PointLight* pointLight;
	static SpotLight* spotLight;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;
	static GLuint skyBoxShader;

	// SkyBox
	static SkyBox* skyBox;

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

	static void readVec3(float* f, glm::vec3 vector) { f[0] = vector.x; f[1] = vector.y; f[2] = vector.z;}
};

#endif
