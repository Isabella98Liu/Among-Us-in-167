#include "DisplayWindow.h"


// Window Properties
int DisplayWindow::width;
int DisplayWindow::height;
const char* DisplayWindow::windowTitle = "GLFW Starter Project";
glm::vec2 DisplayWindow::last_cursor_pos;
glm::vec2 DisplayWindow::cursor_pos;

// Light objects to Render
DirectionalLight* DisplayWindow::directionalLight;
PointLight* DisplayWindow::pointLight;
SpotLight* DisplayWindow::spotLight;

// Objects to Render
Model* DisplayWindow::bear;
Model* DisplayWindow::bunny;
Model* DisplayWindow::sandal;
Model* currObj;

// Key board input booleans
bool is_rotating = false;
bool light_rotating = false;
bool both_rotating = false;

bool light_scaling = false;
bool both_scaling = false;

// Control panel variables
bool show_panel = false;
ImVec4 input_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
float input_f;
float input_f1[3];
float input_f2[3];
float input_f3[3];
float input_f4[3];

ImVec4 input_color2 = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
float input_f5[3];

// Camera Matrices 
// Projection matrix:
glm::mat4 DisplayWindow::projection; 

// View Matrix:
glm::vec3 DisplayWindow::eyePos(0, 0, 20);			// Camera position.
glm::vec3 DisplayWindow::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 DisplayWindow::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 DisplayWindow::view = glm::lookAt(DisplayWindow::eyePos, DisplayWindow::lookAtPoint, DisplayWindow::upVector);

// Shader Program ID
GLuint DisplayWindow::shaderProgram; 
GLuint DisplayWindow::skyBoxShader;

// SkyBox
SkyBox* DisplayWindow::skyBox;

// textures files for skybox
vector<std::string> faces
{
	"Textures/skype_rt.png",
	"Textures/skype_lf.png",
	"Textures/skype_up.png",
	"Textures/skype_dn.png",
	"Textures/skype_ft.png",
	"Textures/skype_bk.png"
};

bool DisplayWindow::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
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

	//----- Temporary add eyeposition shader to here, to be revised in future
	glUseProgram(shaderProgram);
	glUniform3fv(glGetUniformLocation(shaderProgram, "eyePosition"), 1, glm::value_ptr(eyePos));
	glUseProgram(0);

	return true;
}

bool DisplayWindow::initializeObjects()
{
	// Initialize skybox objects
	skyBox = new SkyBox(faces);

	// Initialize light objects
	directionalLight = new DirectionalLight("Models/sphere.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	//directionalLight->translateLightModel(glm::vec3(0.0f, 3.0f, 10.0f));

	pointLight = new PointLight("Models/sphere.obj", glm::vec3(1.0f, 0.87f, 0.0f), glm::vec3(-2.0f, 2.0f, 4.0f), 0.1f, 0.0f, 0.0f);
	//pointLight->getModel()->setMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, pointLight->getColor());
	pointLight->getModel()->setMaterial(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));

	spotLight = new SpotLight("Models/sphere.obj", glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 0.1f, 0.0f, 0.0f, 20.0f);
	spotLight->getModel()->setMaterial(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));
		
	// Initialize models
	bear = new Model("Models/bear.obj");
	bunny = new Model("Models/bunny.obj");
	sandal = new Model("Models/SandalF20.obj");

	// Set material for different  object
	// specular
	bear->setMaterial(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, glm::vec3(0.0f, 1.0f, 1.0f));
	// diffuse
	bunny->setMaterial(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.01f, glm::vec3(0.2f, 1.0f, 0.0f));
	// diffuse + specular
	sandal->setMaterial(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

	// Set bear to be the first to display and to be rotate
	currObj = bear;

	return true;
}

void DisplayWindow::cleanUp()
{
	// Deallcoate the objects.
	delete bear;
	delete bunny;
	delete sandal;

	delete directionalLight;
	delete pointLight;
	delete spotLight;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

GLFWwindow* DisplayWindow::createWindow(int width, int height)
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
	DisplayWindow::resizeCallback(window, width, height);

	return window;
}

void DisplayWindow::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	DisplayWindow::width = width;
	DisplayWindow::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	DisplayWindow::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void DisplayWindow::idleCallback()
{
	// Perform any necessary updates here 
	currObj->update();
}

void DisplayWindow::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the Sky Box
	skyBox->draw(view, projection, skyBoxShader);

	// Render the light objects
	// add a global directional light in order to see the origin color of the material
	directionalLight->sendLightToShader(shaderProgram);		
	//directionalLight->getModel()->draw(view, projection, shaderProgram);

	pointLight->sendLightToShader(shaderProgram);
	pointLight->getModel()->draw(view, projection, shaderProgram);

	//spotLight->sendLightToShader(shaderProgram);
	//spotLight->getModel()->draw(view, projection, shaderProgram);

	// Render the objects
	currObj->draw(view, projection, shaderProgram);
	
	if (show_panel)
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Control Panel");

		// Model editor panel
		ImGui::Text("Model's infomation:");

		glm::vec3 model_color = currObj->getMaterial()->getColor();
		input_color = ImVec4(model_color.x, model_color.y, model_color.z, 1.0f);
		readVec3(input_f1, currObj->getMaterial()->getAmbient());
		readVec3(input_f2, currObj->getMaterial()->getDiffuse());
		readVec3(input_f3, currObj->getMaterial()->getSpecular());
		input_f = currObj->getMaterial()->getShiness();

		ImGui::ColorEdit3("Model's Color", (float*)&input_color); 
		ImGui::SliderFloat3("Ambient", input_f1, 0.0f, 1.0f);
		ImGui::SliderFloat3("Diffuse", input_f2, 0.0f, 1.0f);
		ImGui::SliderFloat3("Specular", input_f3, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &input_f, 0.01f, 1.0f);

		glm::vec3 color_new = glm::vec3(input_color.x, input_color.y, input_color.z);
		glm::vec3 ambient_new = glm::vec3(input_f1[0], input_f1[1], input_f1[2]);
		glm::vec3 diffuse_new = glm::vec3(input_f2[0], input_f2[1], input_f2[2]);
		glm::vec3 specular_new = glm::vec3(input_f3[0], input_f3[1], input_f3[2]);
		GLfloat shininess_new = input_f;

		currObj->setMaterial(ambient_new, diffuse_new, specular_new, shininess_new, color_new);

		// Pointlight editor panel
		ImGui::Text("Pointlight's infomation:");

		glm::vec3 light_color = pointLight->getColor();
		glm::vec3 light_coef = pointLight->getLightCoef();
		input_color2 = ImVec4(light_color.x, light_color.y, light_color.z, 1.0f);
		input_f5[0] = light_coef.x;	input_f5[1] = light_coef.y; input_f5[2] = light_coef.z;

		ImGui::ColorEdit3("PointLight's Color", (float*)&input_color2); 
		ImGui::SliderFloat3("Attenuation ", input_f5, 0.0f, 1.0f);

		light_color = glm::vec3(input_color2.x, input_color2.y, input_color2.z);
		light_coef = glm::vec3(input_f5[0], input_f5[1], input_f5[2]);
		pointLight->setLight(light_coef, light_color);

		ImGui::End();

		// Render dear imgui into screen
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(10, 10, display_w, display_h);
		//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);

}

void DisplayWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

		case GLFW_KEY_1:
			light_rotating = false;
			both_rotating = false;
			light_scaling = false;
			both_scaling = false;
			break;

		case GLFW_KEY_2:
			light_rotating = true;
			both_rotating = false;
			light_scaling = true;
			both_scaling = false;
			break;

		case GLFW_KEY_3:
			light_rotating = false;
			both_rotating = true;
			light_scaling = false;
			both_scaling = true;
			break;
			
		case GLFW_KEY_N:
			currObj->setNormalShadding();
			break;

		case GLFW_KEY_C:
			show_panel = !show_panel;
			break;
		
		default:
			break;
		}
	}
}

void DisplayWindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!light_scaling && !both_scaling)
		currObj->resize(yoffset);
	else if (light_scaling)
		pointLight->scaling(yoffset);
	else if (both_scaling)
	{
		currObj->resize(yoffset);
		pointLight->scaling(yoffset);
	}
}

void DisplayWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
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

void DisplayWindow::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	last_cursor_pos = cursor_pos;
	cursor_pos.x = xpos;
	cursor_pos.y = ypos;
	if (is_rotating && !show_panel)
	{
		if (!light_rotating && !both_rotating)
			currObj->rotate(ballMapping(last_cursor_pos), ballMapping(cursor_pos));
		else if (light_rotating)
			pointLight->rotate(ballMapping(last_cursor_pos), ballMapping(cursor_pos));
		else if (both_rotating)
		{
			currObj->rotate(ballMapping(last_cursor_pos), ballMapping(cursor_pos));
			pointLight->rotate(ballMapping(last_cursor_pos), ballMapping(cursor_pos));
		}
	}
}

glm::vec3 DisplayWindow::ballMapping(glm::vec2 point)
{
	// Mapping the mouse position from 2D to 3D unit sphere
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
