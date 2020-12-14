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
GLuint Window::particleSystemShader;


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
vector<glm::vec3> colorList
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

// Material properties to create astronauts
glm::vec3 astronaut_ambient = glm::vec3(0.8f, 0.8f, 0.8f);
glm::vec3 astronaut_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
glm::vec3 astronaut_specular = glm::vec3(0.5f, 0.5f, 0.5f);
GLfloat astronaut_shininess = 0.323f;
std::vector<glm::vec3> astronauts_colors;

// Other control boolean
GLboolean is_rotating = false;

// Collision detection
std::vector<Physics*> movable_physics;
std::vector<Physics*> static_physics;

// Bot control
GLfloat wait_time = 0.0f;	// wait time to generate next astronaut

// Particle system
std::vector<ParticleSystem*> particleSystems;

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

	particleSystemShader = LoadShaders("shaders/particelSystem.vert", "shaders/particelSystem.frag");
	// Check the shader program.
	if (!particleSystemShader)
	{
		std::cerr << "Failed to initialize ParticleSystem shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Initialize colors for astronauts
	initializeColor();

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
	
	// Initialize environment physic objects for collision detection
	initializeEnvironmentCollision();

	// Initialize astronauts' characters
	initializePlayer();

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

	for (unsigned int i = 0; i < astronauts.size(); i++)
	{
		delete astronaut2Worlds[i];
		delete astronauts[i];
	}

	for (unsigned int i = 0; i < static_physics.size(); i++)
	{
		delete static_physics[i];
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
	nonPlayerControl(deltaTime);
	nonPlayerMovement(deltaTime);
	particleSystemControl(deltaTime);
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

	glUseProgram(particleSystemShader);
	glUniformMatrix4fv(glGetUniformLocation(toonShadingShader, "PV"), 1, false, glm::value_ptr(projection * view));
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
	// if the user is zooming
	camera->scrollControl(xoffset, yoffset);
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

void Window::initializeColor()
{
	for (unsigned int i = 0; i < colorList.size(); i++)
		astronauts_colors.push_back(colorList[i]);
}

void Window::initializePlayer()
{
	// generate a new character as player
	generateCharacter(CHARACTER_PLAYER);

}

void Window::initializeEnvironmentCollision()
{
	glm::vec2 leftUp = glm::vec2(-6.7f, 0.0f);
	glm::vec2 rightUp = glm::vec2(6.7f, 0.0f);
	glm::vec2 leftMid = glm::vec2(-6.7f, 4.8f);
	glm::vec2 rightMid = glm::vec2(6.7f, 4.8f);
	glm::vec2 leftBottom = glm::vec2(-4.8f, 6.7f);
	glm::vec2  rightBottom = glm::vec2(4.8f, 6.7f);

	glm::vec2 leftCenter = glm::vec2(-3.8f, 2.8f);
	glm::vec2 rightCenter = glm::vec2(4.5f, 1.65f);
	GLfloat boxRadius = 0.8f;

	glm::vec3 downNormal = glm::vec3(0, 0, 1);
	glm::vec3 upNormal = glm::vec3(0, 0, -1);
	glm::vec3 leftNormal = glm::vec3(-1, 0, 0);
	glm::vec3 rightNormal = glm::vec3(1, 0, 0);
	glm::vec3 rightUpNormal = glm::normalize(glm::vec3(1, 0, -1));
	glm::vec3 leftUpNormal = glm::normalize(glm::vec3(-1, 0, -1));

	// Up axis wall
	Physics* upWall = new Physics(BOUNDING_LINE, NULL);
	upWall->updateLine(leftUp, rightUp, downNormal);

	// Bottom axis wall
	Physics* bottomWall = new Physics(BOUNDING_LINE, NULL);
	bottomWall->updateLine(leftBottom, rightBottom, upNormal);

	// Left axis wall
	Physics* leftWall = new Physics(BOUNDING_LINE, NULL);
	leftWall->updateLine(leftUp, leftMid, rightNormal);

	// Right axis wall
	Physics* rightWall = new Physics(BOUNDING_LINE, NULL);
	rightWall->updateLine(rightUp, rightMid, leftNormal);

	// Left diag wall
	Physics* leftDiagWall = new Physics(BOUNDING_LINE, NULL);
	leftDiagWall->updateLine(leftMid, leftBottom, rightUpNormal);
	
	// Right diag wall
	Physics* rightDiagWall = new Physics(BOUNDING_LINE, NULL);
	rightDiagWall->updateLine(rightMid, rightBottom, leftUpNormal);

	// Left box
	Physics* leftBox = new Physics(BOUNDING_CIRCLE, NULL);
	leftBox->updateCircle(leftCenter, boxRadius);

	// Right box
	Physics* rightBox = new Physics(BOUNDING_CIRCLE, NULL);
	rightBox->updateCircle(rightCenter, boxRadius);


	static_physics.push_back(upWall);
	static_physics.push_back(bottomWall);
	static_physics.push_back(leftWall);
	static_physics.push_back(rightWall);
	static_physics.push_back(leftDiagWall);
	static_physics.push_back(rightDiagWall);
	static_physics.push_back(leftBox);
	static_physics.push_back(rightBox);
}

void Window::addEnvironmentCollision(Character* character)
{
	// Add  static physic objects to the character astronauts physic objects  
	for (unsigned int i = 0; i < static_physics.size(); i++)
	{
		character->addCollisionPhysic(static_physics[i]);
	}
}

void Window::nonPlayerControl(GLfloat deltaTime)
{
	// Loop through each non player and check life cycle, destroy if needed
	// non-player are stored in [1..9] of astronauts
	std::vector<int> destroy_list;
	for (unsigned int i = 1; i < astronauts.size(); i++)
	{
		GLfloat lifeCycle = astronauts[i]->getLifeCycle();
		lifeCycle -= deltaTime;
		astronauts[i]->setLifeCycle(lifeCycle);

		if (lifeCycle <= 0.0f)
		{
			// if reach the life cycle limit, record the index and destroy the non player later
			destroy_list.push_back(i);
			//printf("\n ADD one character to destroy list\n");
		}
	}


	// Destroy non players that have reach its end of life cycle
	for (unsigned int i = 0; i < destroy_list.size(); i++)
	{
		int index = destroy_list[i];

		//printf("\n Astronauts %d should be destroyed.\n", index);
		deleteCharacter(index);
	}

	// If there are less than CHARACTER_NUM astronauts, produce new astronaut in WAIT_TIME
	if (astronauts.size() < 10)
	{
		if (wait_time - deltaTime > 0)
			wait_time -= deltaTime;
		else
		{
			// if it's time to generate a new astroanut
			generateCharacter(CHARACTER_BOT);
		}
	}
}

void Window::nonPlayerMovement(GLfloat deltaTime)
{
	// non-player starts from 1 of the list
	for (unsigned int i = 1; i < astronauts.size(); i++)
	{
		Character* bot = astronauts[i];
		GLfloat stopGap = bot->getStopGap();
		// if the bot is awake
		if (bot->getStatus() == AWAKE)
		{
			if (stopGap - deltaTime > 0.0f)
			{
				bot->botMove(deltaTime);
				stopGap -= deltaTime;
				bot->setStopGap(stopGap);
			}
			else
			{
				// it's time for frozen the bot
				bot->setStatus(SLEEP);
				bot->setStopGap(getRandFloat(STOP_GAP_MIN, STOP_GAP_MAX));
				//printf("bot %d is set to SLEEP\n", i);
			}
		}
		else if (bot->getStatus() == SLEEP)
		{
			if (stopGap - deltaTime > 0.0f)
			{
				// continue sleeping
				stopGap -= deltaTime;
				bot->setStopGap(stopGap);
			}
			else
			{
				bot->setStopGap(getRandFloat(STOP_GAP_MIN, STOP_GAP_MAX));
				bot->setStatus(AWAKE);
				//printf("bot %d is set to AWAKE\n", i);
			}
		}
	}
}

void Window::particleSystemControl(GLfloat deltaTime)
{
	std::vector<int> destroyList;
	for (unsigned int i = 0; i < particleSystems.size(); i++)
	{
		GLfloat lifeCycle = particleSystems[i]->getLifeCycle();
		
		// this particle system should be destroyed
		if (lifeCycle - deltaTime <= 0.0f)
			destroyList.push_back(i);
		else
			particleSystems[i]->update(deltaTime);
	}

	// destroy the particle system
	for (unsigned int i = 0; i < destroyList.size(); i++)
	{
		int index = destroyList[i];
		worldTransform->deleteChild(particleSystems[index]);
		particleSystems.erase(particleSystems.begin() + index);
	}

}

void Window::generateCharacter(int type)
{
	// Create material for new astronaut
	glm::vec3 color = getColor();
	Material* material = new Material(astronaut_ambient, astronaut_diffuse, astronaut_specular, astronaut_shininess, color);
	// Initialize the astronaut transform and attach character node to it
	Transform* astronaut2World = new Transform();
	Character* astronaut = new Character(frameFiles, LOAD_MODE2);
	// Resize the initial model
	astronaut->update(glm::scale(glm::vec3(1.0f) * 0.06f));

	astronaut->useShader(toonShadingShader);
	astronaut->useMaterial(material);
	astronaut2World->addChild(astronaut);

	// add static physics objects to bot
	addEnvironmentCollision(astronaut);

	// add other characters bouding sphere to this one and this one to other characters
	for (unsigned int i = 0; i < astronauts.size(); i++)
	{
		astronaut->addCollisionPhysic(astronauts[i]->getPhysics());
		astronauts[i]->addCollisionPhysic(astronaut->getPhysics());
	}

	// set character to a randome point in the scene, detect collision at first, if failed keep produce new point
	glm::vec2 randPoint = getRandPoint(-5, 5, 0, 5);
	glm::vec3 randPos = glm::vec3(randPoint.x, -1.8f, randPoint.y);
	//printf("rand position: ( %f %f %f)\n", randPos.x, randPos.y, randPos.z);

	GLboolean res = false;
	while (!res)
	{
		res = astronaut->setPosition(randPos);
		randPoint = getRandPoint(-6, 6, 0, 6);
		randPos = glm::vec3(randPoint.x, -1.8f, randPoint.y);
		//printf("rand position: ( %f %f %f)\n", randPos.x, randPos.y, randPos.z);
	}

	// if this character is a bot, set life cycle and stop gap, set status as awake, set random moveDir
	if (type == CHARACTER_BOT)
	{
		GLfloat lifeCycle = getRandFloat(LIFE_TIME_MIN, LIFE_TIME_MAX);
		astronaut->setLifeCycle(lifeCycle);
		// sleep the new generated bot for 2 seconds before they move
		astronaut->setStopGap(4.0f);
		astronaut->setStatus(SLEEP);
		glm::vec3 randDir;
		glm::vec2 randDir2d = getRandPoint(-1, 1, -1, 1);
		randDir = glm::vec3(randDir2d.x, 0, randDir2d.y);
		astronaut->setFaceDir(randDir);
		//printf("Life Cycle: %f, Stop Gap: %f\n", lifeCycle, stopGap);
	}
	else if (type == CHARACTER_PLAYER)
	{
		// set the player character
		currentAstronaut2World = astronaut2World;
		currentAstronaut = astronaut;
	}

	// add successfully created character to the world
	worldTransform->addChild(astronaut2World);
	astronauts.push_back(astronaut);
	astronaut2Worlds.push_back(astronaut2World);


	// set wait time to generate next astronaut
	wait_time = getRandFloat(WAIT_TIME_MIN, WAIT_TIME_MAX);
	//printf("Set new generation in %f\n\n", wait_time);

	// add particle system effect for new generated bot
	ParticleSystem* particleSystem = new ParticleSystem(astronaut->getPosition(), APPEAR);
	particleSystem->useShader(particleSystemShader);
	particleSystems.push_back(particleSystem);
	// add particle system to the worldtransform
	worldTransform->addChild(particleSystem);
}

void Window::deleteCharacter(int index)
{
	// recycle color
	recycleColor(astronauts[index]->getMatrial()->getColor());

	// add particle system effect for new deletec bot
	ParticleSystem* particleSystem = new ParticleSystem(astronauts[index]->getPosition(), DISAPPEAR);
	particleSystem->useShader(particleSystemShader);
	particleSystems.push_back(particleSystem);
	// add particle system to the worldtransform
	worldTransform->addChild(particleSystem);

	// delete collision object from other player's list
	for (unsigned int j = 0; j < astronauts.size(); j++)
		astronauts[j]->deleteCollisionPhysic(astronauts[index]->getPhysics());

	// delete from worldTransform as well as from atronauts list
	worldTransform->deleteChild(astronaut2Worlds[index]);
	delete astronauts[index];
	delete astronaut2Worlds[index];
	astronauts.erase(astronauts.begin() + index);
	astronaut2Worlds.erase(astronaut2Worlds.begin() + index);
}

glm::vec3 Window::getColor()
{
	// pick the first available color and return it
	glm::vec3 color;
	if (astronauts_colors.size() > 0)
	{
		color = astronauts_colors[0];
		// delete this color from the list
		astronauts_colors.erase(astronauts_colors.begin());	
	}
	return color;
}

void Window::recycleColor(glm::vec3 color)
{
	// recycle color previously used by a character than has disappeared
	astronauts_colors.push_back(color);
}