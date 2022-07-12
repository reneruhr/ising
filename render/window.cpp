#include "window.h"
#include "camera.h"
#include "glfw_callbacks.h"
#include "gui.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Render
{
auto Init(int,int,std::string) -> GLFWwindow*;

Window::Window(std::string title, float ratio , int width)
  : title_(title), width_(width), height_(ratio * width), ratio_(ratio)
{
  window_ = Init(width_, height_, title_);
  auto plane_view = std::make_shared<Camera>();

  std::cout << "Creating window of size " << float(width_) << ", " << float(height_) << '\n';
  plane_view->Projection({0.f, float(width_), 0.f, float(height_) , -1.f, 1.f});
  cameras_.push_back(plane_view);
  active_camera_ = cameras_.back();
  glfwSetWindowUserPointer(window_, reinterpret_cast<void*>(this));
  UpdateViewport();
  GUI::Setup(window_);
}

void Window::UpdateViewport()
{
  static int w, h;
  glfwGetFramebufferSize(window_, &w, &h);
  glViewport(0, 0, w, h);
  //Resize(w,h);
}

void Window::Resize(int w, int h) 
{ 
  active_camera_->Projection({0.f,float(w),0.f, float(h),-1.f,1.f});
}

bool Window::ShouldClose()
{
  return glfwWindowShouldClose(window_);
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
  GUI::NewFrame();
  ProcessInput(window_);
}

void Window::Update()
{
  //UpdateViewport();
  //glClearColor(.3f,.2f,.2f,1.f);
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  for(auto& draw : drawcalls_) draw();		
  GUI::DrawInitial();
  GUI::DrawFinal();
  glfwSwapBuffers(window_);
}

void Window::Exit()
{
  GUI::Clean(); 
  glfwDestroyWindow(window_);
  glfwTerminate();
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
  glfwSwapInterval(0); // vsync
  return window;
}

}
