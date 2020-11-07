#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";
glm::vec2 Window::last_cursor_pos;
glm::vec2 Window::cursor_pos;
bool Window::is_rotating = false;

//// Objects to Render
//Cube * Window::cube;
//PointCloud * Window::cubePoints;
//// extra external obj models to render
//PointCloud* Window::bearPoints;
//PointCloud* Window::bunnyPoints;
//PointCloud* Window::sandalPoints;
//PointCloud* currObj;

Model* Window::bear;
Model* Window::bunny;
Model* Window::sandal;
Model* currObj;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// Shader Program ID
GLuint Window::shaderProgram; 



bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	//cube = new Cube(5.0f);

	// Create a point cloud consisting of cube vertices.
	//cubePoints = new PointCloud("foo", 100);
	//currObj = new PointCloud("", POINT_SIZE);
	//bearPoints = new PointCloud("Models/bear.obj", POINT_SIZE);
	//bunnyPoints = new PointCloud("Models/bunny.obj", POINT_SIZE);
	//sandalPoints = new PointCloud("Models/SandalF20.obj", POINT_SIZE);
	bear = new Model("Models/bear.obj");
	bunny = new Model("Models/bunny.obj");
	sandal = new Model("Models/SandalF20.obj");

	//currObj->setNextObj(bearPoints);
	// Set cube to be the first to display
	//currObj = bearPoints;
	currObj = bear;

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	//delete cube;
	//delete cubePoints;
	//delete bearPoints;
	//delete bunnyPoints;
	//delete sandalPoints;

	delete bear;
	delete bunny;
	delete sandal;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
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

	// Render the objects
	currObj->draw(view, projection, shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

			// switch between the three external obj models
		case GLFW_KEY_F1:
			currObj = bear;
			break;

		case GLFW_KEY_F2:
			currObj = bunny;
			break;

		case GLFW_KEY_F3:
			currObj = sandal;
			break;

		default:
			break;
		}
	}

	//// Check for a key press.
	//if (action == GLFW_PRESS)
	//{
	//	switch (key)
	//	{
	//	case GLFW_KEY_ESCAPE:
	//		// Close the window. This causes the program to also terminate.
	//		glfwSetWindowShouldClose(window, GL_TRUE);				
	//		break;

	//	// switch between the three external obj models
	//	case GLFW_KEY_F1:
	//		currObj->setNextObj(bearPoints);
	//		break;

	//	case GLFW_KEY_F2:
	//		currObj->setNextObj(bunnyPoints);
	//		break;

	//	case GLFW_KEY_F3:
	//		currObj->setNextObj(sandalPoints);
	//		break;

	//	//	control the pointsize of pointcould models
	//	case GLFW_KEY_S:
	//		currObj->updatePointSize(-1.0f);
	//		break;

	//	case GLFW_KEY_L:
	//		currObj->updatePointSize(1.0f);
	//		break;

	//	default:
	//		break;
	//	}
	//}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	currObj->resize(yoffset);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// rotating the model
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
	cursor_pos.x = xpos;
	cursor_pos.y = ypos;
	if (is_rotating)
	{
		//printf("last position: %f,%f  current position: %f,%f \n", last_cursor_pos.x, last_cursor_pos.y, cursor_pos.x, cursor_pos.y);
		currObj->rotate(ballMapping(last_cursor_pos), ballMapping(cursor_pos));
	}
}


// Mapping the mouse position from 2D to 3D unit sphere
glm::vec3 Window::ballMapping(glm::vec2 point)
{
	glm::vec3 pos;
	float d;
	// rescale the x/w.x from [0,1] to [-1,1]
	//pos.x = (point.x / width - 1.0f) * 2;
	//pos.y = (1.0f - point.y / height - 0.5f) * 2;
	pos.x = (2 * point.x - width) / width;
	pos.y = (height - 2 * point.y) / height;
	pos.z = 0;
	d = glm::length(pos);
	d = (d < 1.0f) ? d : 1.0f;
	pos.z = glm::sqrt(1.0f - d*d);
	pos = glm::normalize(pos);
	return pos;
}
