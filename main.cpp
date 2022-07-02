/*
Linux:
g++ -std=c++20 -Wall -g *.cpp render/*.cpp tools/*.cpp -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm 
*/
#include "render/render.h"

#include <chrono>

struct Point2 { float x_,y_; };
struct Point3 { float r_,g_,b_; };

int main() 
{
     using namespace Render;
     using namespace std::chrono_literals;

     
     auto width = 500;
     auto window = Window("Render Test.", 1.f, width );

     auto points = std::vector<Point2>{{0.f,0.f},{1.f,1.f},{2.f,2.f},{3.f,3.f}};
     auto colors = std::vector<Point3>{{0.f,0.f,0.f}, {1.f,1.f,1.f}, {0.5f,.5f,.5f}, {.3f,.3f,.3f}};
     auto buffer = Buffer(DrawMethod::Dynamic, points, colors);
     window.AddDrawCall( 
	  [Points = DrawColoredPoints(buffer), mvp = window.GetCamera()->PV()]()
	       { 
		    Points(mvp);
	       });
     window.Run();
}
