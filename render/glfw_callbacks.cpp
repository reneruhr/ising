#include "glfw_callbacks.h"
#include "window.h"

namespace Render
{
void ProcessInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void KeyCallback(GLFWwindow* window, int key, int code, int act, int mod)
{
}

void FBCallback(GLFWwindow* glfw_window, int w, int h)
{
  //auto window =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
	glViewport(0,0,w,h);
	//window->Resize(w,h);
}

void MouseCallback(GLFWwindow* window, double x, double y)
{
}

void ScrollCallback(GLFWwindow* window, double x, double y)
{
}	
}
