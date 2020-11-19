#define STB_IMAGE_IMPLEMENTATION

#include "main.h"

void error_callback(int error, const char* description)
{
	// Print error.
	std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
	// Set the error callback.
	glfwSetErrorCallback(error_callback);

	// Set the window resize callback.
	glfwSetWindowSizeCallback(window, DisplayWindow::resizeCallback);

	// Set the key callback.
	glfwSetKeyCallback(window, DisplayWindow::keyCallback);

	// Set the scroll callback
	glfwSetScrollCallback(window, DisplayWindow::scrollCallback);

	// Set mouse button callback
	glfwSetMouseButtonCallback(window, DisplayWindow::mouse_button_callback);

	// Set mouse cursor callback
	glfwSetCursorPosCallback(window, DisplayWindow::cursor_position_callback);
}

void setup_opengl_settings()
{
	// Enable depth buffering.
	glEnable(GL_DEPTH_TEST);

	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);

	// Set polygon drawing mode to fill front and back of each polygon.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Set clear color to black.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
		<< std::endl;

	//If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::cout << "Supported GLSL version is: " <<
		glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(void)
{
	// Create the GLFW window.
	GLFWwindow* displayWindow = DisplayWindow::createWindow(640, 480);
	if (!displayWindow)
		exit(EXIT_FAILURE);

	// Print OpenGL and GLSL versions.
	print_versions();

	// Setup callbacks.
	setup_callbacks(displayWindow);

	// Setup OpenGL settings.
	setup_opengl_settings();

	// Initialize the shader program; exit if initialization fails.
	if (!DisplayWindow::initializeProgram())
		exit(EXIT_FAILURE);

	// Initialize objects/pointers for rendering; exit if initialization fails.
	if (!DisplayWindow::initializeObjects())
		exit(EXIT_FAILURE);

	/* NOT NEEDED FOR PROJECT 3
	// ------------ ImGUI part ----------------- 
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(displayWindow, true);
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// ------------------------------------------
	*/

	// Loop while GLFW window should stay open.
	while (!glfwWindowShouldClose(displayWindow))
	{

		// Main render display callback. Rendering of objects is done here. (Draw)
		DisplayWindow::displayCallback(displayWindow);

		// Idle callback. Updating objects, etc. can be done here. (Update)
		DisplayWindow::idleCallback();

	}

	/* NOT NEEDED FOR PROJECT 3
	// Clean up ImGui 
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	*/

	// destroy objects created
	DisplayWindow::cleanUp();

	// Destroy the window.
	glfwDestroyWindow(displayWindow);

	// Terminate GLFW.
	glfwTerminate();

	exit(EXIT_SUCCESS);
}