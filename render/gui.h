#pragma once

class GLFWwindow;
namespace Render
{
namespace GUI
{
  void Setup(GLFWwindow* window);
  void NewFrame();
  void DrawInitial();
  void DrawFinal();
  void Clean();
  void Demo();
}
}
