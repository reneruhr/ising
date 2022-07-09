/*
Linux:
g++ -std=c++20 -Wall -g *.cpp render/*.cpp tools/*.cpp -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm 
*/
#include "render/render.h"
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <array>
#include <numeric>
#include <iostream>

struct Point2 { float x_,y_; 
  friend std::ostream& operator<<(std::ostream& os, const Point2& p)
    {
      os << '(' << p.x_ << ", " << p.y_ << ")\n"; 
      return os;
    }
};

struct Point3 { float r_,g_,b_;
  Point3() : r_(0.f), g_(0.f), b_(0.f) {}
  explicit Point3(Render::Vec3 v) : r_(v[0]), g_(v[1]), b_(v[2]) {}
  friend std::ostream& operator<<(std::ostream& os, const Point3& p)
    {
      os << '(' << p.r_ << ", " << p.g_ << ", " << p.b_ << ")\n"; 
      return os;
    }
};

struct Site {int i_; int j_;};
enum class Nbs { TOP, BOTTOM, LEFT, RIGHT };
auto nbs = std::vector<Nbs> { Nbs::TOP, Nbs::BOTTOM, Nbs::LEFT, Nbs::RIGHT };
constexpr int size {100};
constexpr int N = size*size;
using Flip = bool;
using Row = std::array<Flip, size>;
Site operator+(Site s, Nbs b){
     switch(b){
     case(Nbs::TOP): return {s.i_-1,s.j_};
     case(Nbs::BOTTOM): return {s.i_+1,s.j_};
     case(Nbs::LEFT): return {s.i_,s.j_-1};
     case(Nbs::RIGHT): return {s.i_,s.j_+1};
     }
}

int main() 
{
     float T{4.f};
     int num_iterations=300;

     std::random_device r;
     std::mt19937 mt(r());
     auto RandomProb = [&mt](){ static std::uniform_real_distribution<float> dist(0.f, 1.f); return dist(mt); };
     auto RandomInteger = [&mt](){static std::uniform_int_distribution<> dist(0, size-1); return dist(mt); };
     auto RandomBool = [&mt](){static std::uniform_int_distribution<short> dist(0,1); return dist(mt) > 0; };

     auto Sign = [](Flip f){ return f ? 1 : -1; };
     auto MakeFlip = [](int i) { return i>= 0 ? true : false; };
     auto RandomizeFlip = [&RandomBool](Flip& f) { f = RandomBool(); };
     auto omega = std::array<Row,size>{};
     auto up_flip= MakeFlip(1); 
     auto Boundary = [](Site s) { return s.i_ == 0 || s.i_ == size-1 || s.j_ == 0 || s.j_ == size-1; };
     auto Omega = [&omega](Site s) { return omega[s.i_][s.j_]; };
     auto FlipSite = [&omega](Site s) { omega[s.i_][s.j_]^=true; };
     auto PeriodicBoundary = [&Omega] (Site s){
	  return  Omega({(s.i_+ size) % size, (s.j_ + size) % size});
     };
     auto NbSign = [&Sign, &PeriodicBoundary](Site s, Nbs nb){
	  //return Boundary(s) ? Sign(up_flip) : Sign(Omega(s+nb));
	  return Sign(PeriodicBoundary(s+nb)); 
     };
     auto RandomSite = [&RandomInteger](){
	  return Site{RandomInteger(), RandomInteger()};
     };
     auto EnergyDifference= [&Omega, &NbSign](Site s){
	  using enum Nbs;
	  auto signs = std::vector<int>();
	  using std::begin; using std::end;
	  std::transform(begin(nbs), end(nbs), std::back_inserter(signs), [&s, &NbSign](Nbs x) { return NbSign(s,x); });
	  return 2*Omega(s)* std::accumulate(begin(signs),end(signs), 0);
     };
     auto MeanMagnetization = [&omega, &Sign] (){
	  using std::begin; using std::end;
	  return std::accumulate(
	       begin(omega), end(omega), 0.,
	       [&Sign](int sum, Row& row) { return std::accumulate(
			 begin(row), end(row), sum,
			 [&Sign](int sum, Flip b){ return sum+Sign(b); });
	       })/N;
     };
     auto ColdStart = [&up_flip](Flip& f) { f = up_flip; };
     auto Initialize = [&](auto& Law){
	  for(auto& row : omega)
	       for(auto& f : row)
		    Law(f);
     };	
     auto Run = [&](auto& RenderUpdate){
       for(auto k{0}; k<num_iterations;++k){
	  for(auto l{0}; l<N; ++l){
	       auto s = RandomSite();
	       auto delta = EnergyDifference(s);
	       if(delta <= 0 || RandomProb() < std::exp(-delta/T) ) { FlipSite(s);}
	  }
	  RenderUpdate();
       }
     };


//////////////////////////////////// RENDER /////////////////////////////

     using namespace Render;
     using namespace std::chrono_literals;
     
     auto width = 500;
     auto window = Window("Monte Carlo Metropolis Simulation.", 1.f, width );

     auto points = std::vector<Point2>(N);
     auto colors   = std::vector<Point3>(N);

     auto SitesToBuffer = [gap = float(width)/size, &points](){
       for(auto i(0); i < size; ++i)
	 for(auto j(0); j < size; ++j){
	   points[size*i+j] = {i*gap+gap/2, j*gap+gap/2};
	 }
     };
     
     auto StatesToBuffer = [&Omega, &colors](Color a = Color::CornFlowerBlue, Color b = Color::Tomato){
       for(auto i(0); i < size; ++i)
	 for(auto j(0); j < size; ++j)
	   colors[size*i+j] = Point3(MakeRGB(Omega({i,j}) ? a : b));
     };


     Initialize(RandomizeFlip); 

     SitesToBuffer();
     StatesToBuffer();
     auto buffer = Buffer(DrawMethod::Dynamic, points, colors);
     auto UpdateBuffer = [&buffer,&colors, &StatesToBuffer]()
       {
	 StatesToBuffer();
	 buffer.Update(colors, 1);
       };
     auto DrawPoints = [Points = DrawColoredPoints(buffer), mvp = window.GetCamera()->PV()]()
       { 
	 Points(mvp);
       };
     window.AddDrawCall(DrawPoints);
     auto RenderUpdate = [&window, &UpdateBuffer]()
       {
	 window.UpdateEvents();
	 UpdateBuffer();
	 window.Update();
       };
     Run(RenderUpdate);
}
