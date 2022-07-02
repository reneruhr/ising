#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Render
{

void ProcessInput(GLFWwindow* window);
void KeyCallback(GLFWwindow* window, int key, int code, int act, int mod);
void FBCallback(GLFWwindow* window, int w, int h);
void MouseCallback(GLFWwindow* window, double x, double y);
void ScrollCallback(GLFWwindow* window, double x, double y);

}
