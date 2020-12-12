#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Project 4 Among Us in 167";
bool Window::keys[1024];
glm::vec2 Window::last_cursor_pos;
glm::vec2 Window::cursor_pos;
GLfloat Window::xChange = 0.0f;
GLfloat Window::yChange = 0.0f;
GLfloat Window::deltaTime = 0.0f;
GLfloat  Window::lastTime = 0.0f;

// Lights, Textures, Materialss
DirectionalLight* directionalLight;
Material* lobbyMaterial;
GLuint lobbyTexture;

// Camera Matrices 
Camera* Window::camera;
glm::mat4 Window::view;
glm::mat4 Window::projection;
glm::vec3 Window::eyePos;

// Shader Program ID
GLuint Window::objectShader; 
GLuint Window::toonShadingShader;

// Scene Graph
/*
							World
				/			  |		......		\
	lobby2World			astronaut2World1		astronaut2World10
	(lobby.geo)			(astronaut1.cha)		(astronaut10.cha)
*/
Transform* worldTransform;
Transform* lobby2World;
Geometry* lobby;
std::vector<Transform*> astronaut2Worlds;	// all astronaut transforms
std::vector<Character*> astronauts;			// all astronauts
Transform* currentAstronaut2World;			// player's astronaut transform
Character* currentAstronaut;				// player's astronaut 

// Frame files for astronauts
vector<std::string> frameFiles
{
	"Models/amongus_astro_still.obj",
	"Models/amongus_astro_moving1.obj",
	"Models/amongus_astro_moving2.obj"
};
// Colors for astronauts
vector<glm::vec3> colors
{
	glm::vec3((float)62 / 255, (float)71 / 255, (float)78 / 255),	// black
	glm::vec3((float)19 / 255, (float)46 / 255, (float)209 / 255),	// blue
	glm::vec3((float)113 / 255, (float)73 / 255, (float)29 / 255),	// brown
	glm::vec3((float)57 / 255, (float)254 / 255, (float)221 / 255),	// cyan
	glm::vec3((float)19 / 255, (float)128 / 255, (float)44 / 255),	// dark green
	glm::vec3((float)78 / 255, (float)239 / 255, (float)56 / 255),	// lime
	glm::vec3((float)241 / 255, (float)125 / 255, (float)12 / 255),	// orange
	glm::vec3((float)236 / 255, (float)84 / 255, (float)187 / 255),	// pink
	glm::vec3((float)108 / 255, (float)47 / 255, (float)188 / 255),	// purple
	glm::vec3((float)197 / 255, (float)18 / 255, (float)17 / 255)	// red
};

// Other control boolean
GLboolean is_rotating = false;


bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	objectShader = LoadShaders("shaders/BaseObject.vert", "shaders/BaseObject.frag");
	// Check the shader program.
	if (!objectShader)
	{
		std::cerr << "Failed to initialize BaseObject shader program" << std::endl;
		return false;
	}

	toonShadingShader = LoadShaders("shaders/ToonShading.vert", "shaders/ToonShading.frag");
	// Check the shader program.
	if (!toonShadingShader)
	{
		std::cerr << "Failed to initialize ToonShading shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Initialize directional light
	directionalLight = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	// Initialize materials
	lobbyMaterial = new Material(glm::vec3(0.8f, 0.8f, 0.8f),
								glm::vec3(0.8f, 0.8f, 0.8f),
								glm::vec3(0.5f, 0.5f, 0.5f),
								0.323f,
								glm::vec3(1.0f, 1.0f, 1.0f));

	// Initialize textures
	lobbyTexture = loadTexture("Models/cse167f20-project4-lobby/amongus_lobby.png");

	// Initialize a new Camera
	camera = new Camera(glm::vec3(0.0f, 10.0f, 9.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f), 
						-90.0f, -45.0f, 10.0f, 0.5f);

	// Initialize the world transform
	worldTransform = new Transform();

	// Initialize the lobby transform and attach lobby geometry to it
	lobby2World = new Transform();
	lobby = new Geometry("Models/cse167f20-project4-lobby/amongus_lobby.obj", LOAD_MODE2);
	lobby->useShader(objectShader);
	lobby->useMaterial(lobbyMaterial);
	lobby->useTexture(lobbyTexture);
	lobby2World->addChild(lobby);
	worldTransform->addChild(lobby2World);

	// Initialize astronauts' characters
	initializeCharacters();

	return true;
}

void Window::cleanUp()
{
	// Delete Objects
	delete directionalLight;
	delete lobbyMaterial;
	delete camera;
	
	delete worldTransform;
	delete lobby2World;
	delete lobby;

	for (unsigned int i = 0; i < CHARACTER_NUM; i++)
	{
		delete astronaut2Worlds[i];
		delete astronauts[i];
	}

	// Delete the shader program.
	glDeleteProgram(objectShader);
	glDeleteProgram(toonShadingShader);
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

}

void Window::displayCallback(GLFWwindow* window)
{	
	// Calculate the deltatime
	GLfloat currentTime = (GLfloat)glfwGetTime();		// SDL_GetPerformanceCounter()
	deltaTime = currentTime - lastTime;			// (currentTime - lastTime) * 1000 / SDL_GetPerformancefrequency();
	lastTime = currentTime;

	// Send keyboard input information to player's character
	currentAstronaut->keyControl(keys, deltaTime);

	// Clear the color and depth buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Obtaint the latest Camera informaiton (direction and position)
	view = camera->calculateViewMatrix();
	eyePos = camera->getPosition();
	glUseProgram(objectShader);
	glUniformMatrix4fv(glGetUniformLocation(objectShader, "PV"), 1, false, glm::value_ptr(projection * view));
	glUniform3fv(glGetUniformLocation(objectShader, "eyePos"), 1, glm::value_ptr(eyePos));
	glUseProgram(0);

	glUseProgram(toonShadingShader);
	glUniformMatrix4fv(glGetUniformLocation(toonShadingShader, "PV"), 1, false, glm::value_ptr(projection * view));
	glUniform3fv(glGetUniformLocation(toonShadingShader, "eyePos"), 1, glm::value_ptr(eyePos));
	glUseProgram(0);

	// Send light information to shader
	directionalLight->sendLightToShader(objectShader);
	directionalLight->sendLightToShader(toonShadingShader);
	
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
	// if the user is resizing the lobby object
	float scale = 1.0f + yoffset * 0.02f;
	worldTransform->update(glm::scale(glm::vec3(scale)));
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// detect whether user is pressing the left mouse button -> rotating the camera
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		is_rotating = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		is_rotating = false;
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	last_cursor_pos = cursor_pos;
	cursor_pos.x = (GLfloat)xpos;
	cursor_pos.y = (GLfloat)ypos;

	xChange = cursor_pos.x - last_cursor_pos.x;
	yChange = last_cursor_pos.y - cursor_pos.y;
	
	// if user is rotating the camera
	if(is_rotating)
		worldTransform->update(glm::rotate(0.02f, 
			glm::cross(trackBallMapping(last_cursor_pos), trackBallMapping(cursor_pos))));

}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
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

void Window::initializeCharacters()
{
	// Initialize properties to create basic materials for astronauts
	glm::vec3 ambient = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
	GLfloat shininess = 0.323f;

	// Creat CHARACTER_NUM astronauts
	for (unsigned int i = 0; i < CHARACTER_NUM; i++)
	{
		// Initialize material for this astronaut
		Material* material = new Material(ambient, diffuse, specular, shininess, colors[i]);

		// Initialize the astronaut transform and attach character node to it
		Transform* astronaut2World = new Transform();
		Character* astronaut = new Character(frameFiles, LOAD_MODE2);
		// Resize the initial model
		astronaut->update(glm::scale(glm::vec3(1.0f) * 0.06f));
		astronaut->setPosition(glm::vec3(1.6f * i, 0.0f, 0.0f));

		astronaut->useShader(toonShadingShader);
		astronaut->useMaterial(material);

		astronaut2World->addChild(astronaut);
		worldTransform->addChild(astronaut2World);

		astronauts.push_back(astronaut);
		astronaut2Worlds.push_back(astronaut2World);
	}

	// set the player character
	currentAstronaut2World = astronaut2Worlds[0];
	currentAstronaut = astronauts[0];

}