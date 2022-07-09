#include "window.h"
#include "camera.h"
#include "glfw_callbacks.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Render
{

auto Init(int,int,std::string) -> GLFWwindow*;

Window::Window(std::string title, float ratio , int width)
:
title_(title), width_(width), height_(ratio * width), ratio_(ratio)
{
	window_ = Init(width_, height_, title_);
	auto plane_view = std::make_shared<Camera>();
/*
(0,height)
     |
     |
(0,0)|_______ (width, 0)

 */
	std::cout << "Creating window of size " << float(width_) << ", " << float(height_) << '\n';
	plane_view->Projection({0.f,float(width_),0.f, float(height_) ,-1.f,1.f});
	cameras_.push_back(plane_view);
	active_camera_ = cameras_.back();
	glfwSetWindowUserPointer(window_, reinterpret_cast<void*>(this));
	glfwGetFramebufferSize(window_, &width_, &height_);
	Resize(width_,height_);
}

void Window::Run()
{
  while(!glfwWindowShouldClose(window_))
  {
    UpdateEvents();
    Update();
  }
}

void Window::UpdateEvents()
{
    glfwPollEvents();
    ProcessInput(window_);
}

void Window::Update()
{
  glClearColor(.3f,.2f,.2f,1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  for(auto& draw : drawcalls_) draw();		
  glfwSwapBuffers(window_);
}

void Window::Exit()
{
  glfwTerminate();
}

void Window::Resize(int w, int h) 
{ 
	width_ = w; 
	height_ = h; 
	active_camera_->Projection({0.f,float(width_),0.f, float(height_),-1.f,1.f});
}

void Window::AddDrawCall(std::function<void(void)> fct)
{
	drawcalls_.push_back(std::move(fct));
}

auto Init(int w, int h, std::string title) -> GLFWwindow* 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);	
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	auto window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);

	if(!window)
	{ 
		std::cout << "Failed Creating GLFW window\n";
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);	
	glfwSetFramebufferSizeCallback(window, FBCallback);
	
	glewExperimental = GL_TRUE;
	if(auto e = glewInit(); GLEW_OK != e)
	{	
		std::cout << glewGetErrorString(e);
		glfwTerminate();
		return nullptr;
	}

	return window;
}

}
