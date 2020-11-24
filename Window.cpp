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

// Scene Graph
Transform* worldTransform;
Transform* projectionTransform;
Transform* viewTransform;
Transform* carrousel2World;
std::vector<Transform*> suspension2Carrousels;
std::vector<Transform*> rabbit2Suspensions;
GLfloat suspensionMoveOffset = 0.0f;
GLfloat suspensionMoveEdge = 1.0f;
GLint suspensionMoveDirection = 1;
GLboolean is_animate_l1 = true;
GLboolean is_animate_l2 = true;
GLboolean is_animate_l3 = true;

// Materials, Lights, Textures
Material* basicMaterial;
Material* whiteMaterial;
Material* goldMaterial;
Material* silverMaterial;
Material* pinkMaterial;
DirectionalLight* directionalLight;
GLuint brickTexture;

// Camera Matrices 
Camera* Window::camera;
glm::mat4 Window::view;
glm::mat4 Window::projection;
glm::vec3 Window::eyePos;

// Shader Program ID
GLuint Window::objectShader; 
GLuint Window::skyBoxShader;
GLuint Window::envMapShader;
GLuint Window::textureShader;


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
	objectShader = LoadShaders("shaders/BaseObject.vert", "shaders/BaseObject.frag");
	// Check the shader program.
	if (!objectShader)
	{
		std::cerr << "Failed to initialize BaseObject shader program" << std::endl;
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
		std::cerr << "Failed to load EnvMapping shader program" << std::endl;
		return false;
	}

	// Load shader for texture mapping
	textureShader = LoadShaders("shaders/TextureObject.vert", "shaders/TextureObject.frag");
	// Check the shader program.
	if (!textureShader)
	{
		std::cerr << "Failed to load TextureObject shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Initialize materials and lights
	basicMaterial = new Material(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f),
		glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f, glm::vec3(1.0f, 1.0f, 1.0f));
	whiteMaterial = new Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.55f, 0.55f, 0.55f),
		glm::vec3(0.7f, 0.7f, 0.7f), 0.25f, glm::vec3(1.0f, 1.0f, 1.0f));
	goldMaterial = new Material(glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f),
		glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
	silverMaterial = new Material(glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f),
		glm::vec3(0.508273f, 0.508273f, 0.508273f), 0.6f, glm::vec3(1.0f, 1.0f, 1.0f));
	pinkMaterial = new Material(glm::vec3(0.4f, 0.4f, 0.4), glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(0.25f, 0.25f, 0.25f), 1.0f, glm::vec3(1.0f, 0.4f, 0.7f));

	directionalLight = new DirectionalLight("Models/sphere.obj", 1, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	brickTexture = loadTexture("Textures/brick3.jpg");

	// Initialize skybox objects
	skyBox = new SkyBox(faces);

	// Initialize a new Camera
	camera = new Camera(glm::vec3(0.0f, 0.0f, 80.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f), 
						-90.0F, 0.0f, 5.0f, 0.5f);

	worldTransform = new Transform();

	// Initialize carrousel transform
	initializeCarrousel();
	carrousel2World->update(glm::translate(glm::vec3(0.0f, -2.0f, 0.0f)));
	worldTransform->addChild(carrousel2World);

	// Add disco ball transform
	Transform* disco2World = new Transform();
	Sphere* discoBall = new Sphere(24, 24, 5);
	discoBall->useTexture(skyBox->getTexture());
	discoBall->useShader(envMapShader);
	disco2World->addChild(discoBall);
	disco2World->update(glm::translate(glm::vec3(0.0f, 25.0f, 0.0f)));
	worldTransform->addChild(disco2World);

	Transform* ground2World = initializeGround();
	worldTransform->addChild(ground2World);

	return true;
}

void Window::cleanUp()
{
	// Delete Objects
	delete skyBox;
	delete camera;

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
	glUseProgram(objectShader);
	glUniform3fv(glGetUniformLocation(objectShader, "eyePos"), 1, glm::value_ptr(eyePos));
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "PV"), 1, false, glm::value_ptr(projection * view));
	glUseProgram(0);

	glUseProgram(envMapShader);
	glUniform3fv(glGetUniformLocation(envMapShader, "eyePos"), 1, glm::value_ptr(eyePos));
	glUniformMatrix4fv(glGetUniformLocation(envMapShader, "PV"), 1, false, glm::value_ptr(projection * view));
	glUseProgram(0);

	glUseProgram(textureShader);
	glUniformMatrix4fv(glGetUniformLocation(textureShader, "PV"), 1, false, glm::value_ptr(projection * view));
	glUseProgram(0);

	// L1 animation: carrousel rotate around its vertical axis
	if (is_animate_l1)
	{
		carrousel2World->update(glm::rotate(2.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
	if (deltaTime > 0.1f) return;	// first load produce large deltaTime, discard, to be revised in the future
	suspensionMoveOffset += deltaTime * suspensionMoveDirection;
	if (suspensionMoveOffset > suspensionMoveEdge || suspensionMoveOffset < -suspensionMoveEdge)
		suspensionMoveDirection = (-1) * suspensionMoveDirection;
	// L2 animation: suspensions move up and down
	if (is_animate_l2)
	{
		for (int i = 0; i < suspension2Carrousels.size(); i++)
		{
			int direction = ((i % 2) * 2 - 1) * suspensionMoveDirection;
			suspension2Carrousels[i]->update(glm::translate(glm::vec3(0.0f, direction * deltaTime * 1.4f, 0.0f)));
		}
	}
	// L3 animation: rabbit moves around its own vertival axis
	if (is_animate_l3)
	{
		for (int i = 0; i < rabbit2Suspensions.size(); i++)
		{
			int direction = ((i % 2) * 2 - 1) * suspensionMoveDirection;
			rabbit2Suspensions[i]->update(glm::rotate(direction * deltaTime * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
		}
	}
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

	//basicMaterial->sendMatToShader(objectShader);
	directionalLight->sendLightToShader(objectShader);
		
	// Render the Scene Graph Tree
	worldTransform->draw(glm::mat4(1));

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
				
				case GLFW_KEY_1:
					is_animate_l1 = !is_animate_l1;
					break;
				
				case GLFW_KEY_2:
					is_animate_l2 = !is_animate_l2;
					break;

				case GLFW_KEY_3:
					is_animate_l3 = !is_animate_l3;
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

Transform* Window::initializeCarrousel()
{
	// L1 (Trans carrousel2World + Geo Roof + Geo pillar)
	carrousel2World = new Transform();
	Geometry* Roof = new Geometry("Models/cone.obj", 2);
	Roof->useShader(objectShader);
	Roof->useMaterial(goldMaterial);

	Geometry* Pillar = new Geometry("Models/cylinder.obj", 2);
	Pillar->useShader(objectShader);
	Pillar->useMaterial(goldMaterial);

	carrousel2World->addChild(Roof);
	carrousel2World->addChild(Pillar);

	// Adjust
	Roof->rescale(glm::vec3(3.0f, 0.6f, 3.0f));
	Pillar->rescale(glm::vec3(0.3f, 2.5f, 0.3f));
	Roof->translate(glm::vec3(0.0f, 15.0f, 0.0f));

	// L2 (Trans suspension2Carrousel + Geo suspension)
	int suspensionCount = 6;
	int radius = 15.0f;
	//std::vector<Transform*> suspension2Carrousels;
	for (int i = 0; i < suspensionCount; i++)
	{
		Transform* suspension2Carrousel = new Transform();
		Geometry* suspension = new Geometry("Models/cylinder.obj", 2);
		suspension->useShader(objectShader);
		suspension->useMaterial(silverMaterial);
		suspension2Carrousel->addChild(suspension);
		suspension->rescale(glm::vec3(0.08f, 1.7f, 0.08f));

		double sinx = sin(i * 2 * glm::pi<double>() / suspensionCount);
		double cosx = cos(i * 2 * glm::pi<double>() / suspensionCount);
		suspension2Carrousel->update(glm::translate(glm::vec3(sinx * radius, 0.0f, cosx * radius)));

		suspension2Carrousels.push_back(suspension2Carrousel);
	}
	carrousel2World->addChilds(suspension2Carrousels);

	// L3 (Trans rabbit2Suspension + Geo rabbit)
	for (int i = 0; i < suspension2Carrousels.size(); i++)
	{
		Transform* rabbit2Suspension = new Transform();
		Geometry* rabbit = new Geometry("Models/bunny.obj", 1);
		rabbit->useShader(objectShader);
		rabbit->useMaterial(pinkMaterial);
		//Geometry* rabbit = new Geometry("Models/cone.obj");

		rabbit2Suspension->addChild(rabbit);
		rabbit->rescale(glm::vec3(0.5f, 0.5f, 0.5f));
		rabbit->translate(glm::vec3(0.0f, -1.0f, 0.0f));

		rabbit2Suspension->update(glm::translate(glm::vec3(0.0f, -2.5f, 0.0f)));

		rabbit2Suspensions.push_back(rabbit2Suspension);
		suspension2Carrousels[i]->addChild(rabbit2Suspension);
	}

	return carrousel2World;
}

Transform* Window::initializeGround()
{
	Transform* ground2World = new Transform();
	Geometry* Ground = new Geometry("Models/cube.obj", 2);
	//Ground->useShader(objectShader);
	Ground->useShader(textureShader);
	Ground->useMaterial(basicMaterial);
	Ground->useTexture(brickTexture);

	ground2World->addChild(Ground);

	Ground->rescale(glm::vec3(50.0f, 0.1f, 50.0f));
	Ground->translate(glm::vec3(0.0f, -20.0f, 0.0f));
	
	return ground2World;
}

GLuint Window::loadTexture(std::string fileName)
{
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bi-linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // for both filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture edge mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << fileName << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}