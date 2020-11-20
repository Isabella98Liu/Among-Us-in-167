#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Project 3 Amusement Park Ride";
bool Window::keys[1024];
glm::vec2 Window::last_cursor_pos;
glm::vec2 Window::cursor_pos;
GLfloat xChange = 0.0f;
GLfloat yChange = 0.0f;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Light objects to Render
DirectionalLight* Window::directionalLight;
PointLight* Window::pointLight;
SpotLight* Window::spotLight;

// Objects to Render
Sphere* Window::discoBall;
BaseObject* currObj;

// Camera Matrices 
Camera* Window::camera;
glm::mat4 Window::view;
glm::mat4 Window::projection;
glm::vec3 Window::eyePos;

// Shader Program ID
GLuint Window::objectShader; 
GLuint Window::skyBoxShader;
GLuint Window::envMapShader;

// SkyBox
SkyBox* Window::skyBox;

// textures files for skybox
vector<std::string> faces
{
	"Textures/sh_rt.png",
	"Textures/sh_lf.png",
	"Textures/sh_up.png",
	"Textures/sh_dn.png",
	"Textures/sh_ft.png",
	"Textures/sh_bk.png"
};

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	objectShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	// Check the shader program.
	if (!objectShader)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Load shader for SkyBox
	skyBoxShader = LoadShaders("shaders/SkyBox.vert", "shaders/SkyBox.frag");
	// Check the shader program.
	if (!skyBoxShader)
	{
		std::cerr << "Failed to initialize SkyBox shader program" << std::endl;
		return false;
	}

	// Load shader for environment mapping
	envMapShader = LoadShaders("shaders/EnvMapping.vert", "shaders/EnvMapping.frag");
	// Check the shader program.
	if (!envMapShader)
	{
		std::cerr << "Failed to load environment mapping shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Initialize skybox objects
	skyBox = new SkyBox(faces);

	// Initialize a new Camera
	camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f), 
						0.0f, 0.0f, 5.0f, 0.5f);

	discoBall = new Sphere(24, 24, 5);


	return true;
}

void Window::cleanUp()
{
	// Delete Objects
	delete skyBox;
	delete camera;
	delete discoBall;

	// Delete the shader program.
	glDeleteProgram(objectShader);
	glDeleteProgram(skyBoxShader);
	glDeleteProgram(envMapShader);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any necessary updates here 
	currObj->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Calculate the deltatime
	GLfloat currentTime = (GLfloat)glfwGetTime();		// SDL_GetPerformanceCounter()
	deltaTime = currentTime - lastTime;			// (currentTime - lastTime) * 1000 / SDL_GetPerformancefrequency();
	lastTime = currentTime;

	camera->keyControl(keys, deltaTime);
	
	// Obtaint the latest Camera informaiton (direction and position)
	view = camera->calculateViewMatrix();
	eyePos = camera->getPosition();

	// Render the Sky Box
	skyBox->draw(view, projection, skyBoxShader);

	// Render the disco ball using environment mapping
	discoBall->useTexture(skyBox->getTexture());
	discoBall->useShader(envMapShader);
	discoBall->draw(view, projection, eyePos);


	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);

}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;

			switch (key)
			{
				case GLFW_KEY_ESCAPE:
					// Close the window. This causes the program to also terminate.
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;

				default:
					break;
			}
		}
		else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	last_cursor_pos = cursor_pos;
	cursor_pos.x = (GLfloat)xpos;
	cursor_pos.y = (GLfloat)ypos;

	xChange = cursor_pos.x - last_cursor_pos.x;
	yChange = last_cursor_pos.y - cursor_pos.y;

	camera->mouseControl(xChange, yChange);

}

glm::vec3 Window::ballMapping(glm::vec2 point)
{
	// Mapping the mouse position from 2D to 3D unit sphere
	glm::vec3 pos;
	float d;
	pos.x = (2 * point.x - width) / width;
	pos.y = (height - 2 * point.y) / height;
	pos.z = 0;
	d = glm::length(pos);
	d = (d < 1.0f) ? d : 1.0f;
	pos.z = glm::sqrt(1.0f - d*d);
	pos = glm::normalize(pos);
	return pos;
}
