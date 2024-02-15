/*
   Create a GLFW window
*/

/*
   This is a simple example of how to create a window using GLFW.
   The window is created with a size of 640x480 pixels and a title of "Hello World".
   The window is cleared to a black color and then the front and back buffers are swapped.
   The program will continue to run until the user closes the window.

*/

#include "glad.h"
#include <GLFW/glfw3.h>

/*
	"Hello Window" code from https://learnopengl.com/Getting-started/Hello-Window
*/

int main()
{
	// Initialize GLFW
	glfwInit();

	// Set the version of OpenGL to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (window == NULL)
	{
		// If the window fails to be created, terminate GLFW and return an error code
		glfwTerminate();
		return -1;
	}

	// Make the window the current context
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// If the OpenGL function pointers fail to load, terminate GLFW and return an error code
		glfwTerminate();
		return -1;
	}

	// Run the program until the window is closed
	while (!glfwWindowShouldClose(window))
	{
		// Clear the window
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap the front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Terminate GLFW
	glfwTerminate();

	// Return 0 to indicate success
	return 0;
}