//
// Linux:
//
// g++ -std=c++20 -Wall *.cpp render/*.cpp tools/*.cpp -render/imgui/*.cpp lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm 
// Mac:
// ~/g++11 -std=c++20 -Wall *.cpp render/*.cpp tools/*.cpp render/imgui/*.cpp -o ising -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw -framework OpenGL -lglew

#include "render/render.h"
#include "render/gui.h"
#include "render/imgui/imgui.h"
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <array>
#include <numeric>
#include <iostream>
#include <cmath>

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
constexpr int size {200};
constexpr int N = size*size;
using Flip = bool;
using Row = std::array<Flip, size>;
Site operator+(Site s, Nbs b){
     switch(b){
     case(Nbs::TOP): return {s.i_-1,s.j_};
     case(Nbs::BOTTOM): return {s.i_+1,s.j_};
     case(Nbs::LEFT): return {s.i_,s.j_-1};
     case(Nbs::RIGHT): return {s.i_,s.j_+1};
     default: return {0,0};
     }
}

int main() 
{
  float temp{2.f}, J{1.f}, K{1.f};
  int num_iterations{1000};
  int current_iteration{0};

  std::random_device r;
  std::mt19937 mt(r());
  auto RandomProb = [&mt](){ static std::uniform_real_distribution<float> dist(0.f, 1.f); return dist(mt); };
  auto RandomInteger = [&mt](){static std::uniform_int_distribution<> dist(0, size-1); return dist(mt); };
  auto RandomBool = [&mt](){static std::uniform_int_distribution<short> dist(0,1); return dist(mt) > 0; };

  auto Sign = [](Flip f){ return f ? 1 : -1; };
  auto Weight = [&J,&K](Nbs nb)
    {
     switch(nb){
     case(Nbs::TOP): return K; 
     case(Nbs::BOTTOM): return K; 
     case(Nbs::LEFT): return J;
     case(Nbs::RIGHT): return J;
     default: return 0.f;
     }
    };
  auto MakeFlip = [](int i) { return i>= 0 ? true : false; };
  auto RandomizeFlip = [&RandomBool](Flip& f) { f = RandomBool(); };
  auto omega = std::array<Row,size>{};
  auto up_flip= MakeFlip(1); 
  auto ColdStart = [&up_flip](Flip& f) { f = up_flip; };
  auto Omega = [&omega](Site s) { return omega[s.i_][s.j_]; };
  auto FlipSite = [&omega](Site s) { omega[s.i_][s.j_]^=true; };
  auto PeriodicBoundary = [&Omega] (Site s){
    return  Omega({(s.i_+ size) % size, (s.j_ + size) % size});
  };
  auto NbSign = [&Sign, &PeriodicBoundary](Site s, Nbs nb){
    return Sign(PeriodicBoundary(s+nb)); 
  };
  auto RandomSite = [&RandomInteger](){
    return Site{RandomInteger(), RandomInteger()};
  };
  auto EnergyDifference= [&Omega, &NbSign, &Sign, &Weight](Site s){
    using enum Nbs;
    auto signs = std::vector<float>();
    using std::begin; using std::end;
    std::transform(begin(nbs), end(nbs), std::back_inserter(signs),
		   [&s, &NbSign, &Weight](Nbs x) { return Weight(x)*NbSign(s,x); });
    return 2 * Sign(Omega(s)) * std::accumulate(begin(signs),end(signs), 0.f);
  };
  auto M = [&omega, &Sign] (){
    using std::begin; using std::end;
    return std::accumulate(
      begin(omega), end(omega), 0.f,
      [&Sign](float sum, Row& row) { return std::accumulate(
	  begin(row), end(row), sum,
	  [&Sign](float sum, Flip b){ return sum+Sign(b); });
      });
  };
  auto MeanMagnetization = [&M] (){
    return M()/N;
  };
  auto E = [&omega, &Sign, &K, &J] (){
    using std::begin; using std::end;
     auto energy =
       std::accumulate(begin(omega), end(omega), 0.f,
	[&Sign, &K](float sum, Row& row) { 
	for(auto i{0}; i < size; ++i)
	  sum =- K * Sign(row[i])*Sign(row[(i+1+size)%size]);
	return sum;
      });
     auto Col = [&omega, &Sign](int i)
       {
	 return [&omega,&Sign, i](int j)
	 {
	   return Sign(omega[(size+j)%size][(i+size)%size]);
	 };
       };
     for(auto i{0}; i<size; ++i)
       for(auto j{0}; j<size; ++j)
	 energy-= J * Col(i)(j)*Col(i+1)(j);
     return energy;
  };
  auto EnergyDensity = [&E] (){
    return E()/N;
  };

  auto Var = [](auto& data)
    {
      auto n = std::size(data);
      return std::accumulate(std::begin(data), std::end(data), 0.f, [](float sum, float f) { return sum+f*f;})/n
	      - std::pow(std::accumulate(std::begin(data), std::end(data), 0.f)/n,2.f);
    };
  
  auto Initialize = [&](auto& Law){
    for(auto& row : omega)
      for(auto& f : row)
	Law(f);
  };	
  auto Run = [&](auto& RenderUpdate){
    for(;current_iteration < num_iterations; ++current_iteration){
      for(auto l{0}; l<N; ++l){
	auto s = RandomSite();
	auto delta = EnergyDifference(s);
	if(delta < 0 || RandomProb() < std::exp(-delta/temp) ) { FlipSite(s);}
      }
      if(RenderUpdate()) return;
    }
  };

//////////////////////////////////// RENDER /////////////////////////////

  using namespace Render;
  using namespace std::chrono_literals;
     
  auto width = 1000;
  auto window = Window("Monte Carlo Metropolis Simulation.", 1.f, width );

  auto points = std::vector<Point2>(N);
  auto colors   = std::vector<Point3>(N);
  auto point_size{8.f};
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
  auto DrawPoints = [Points = DrawColoredPoints(buffer), mvp = window.GetCamera()->PV(), &point_size]()
    { 
      Points(mvp, point_size);
    };
  static bool pause_simulation = false;
  auto DrawGui = [&]()
    {
      static bool loop = true;
      ImGui::Begin("Controls");

      ImGui::Checkbox("Pause Simulation", &pause_simulation);
      ImGui::Checkbox("Loop", &loop);
      ImGui::SameLine();
      ImGui::Text("(Iterations:= %d)", current_iteration);
      ImGui::SliderFloat("Temperature", &temp, 0.0f, 10.0f); 
      ImGui::SliderFloat("Hor. bond", &J, 0.0f, 10.0f); 
      ImGui::SliderFloat("Vert. bond", &K, 0.0f, 10.0f); 
      ImGui::Separator();
      ImGui::Text("Mean Magnetization= %f", MeanMagnetization() );
      ImGui::Text("EnergyDensity= %f", EnergyDensity() );
      if(loop and current_iteration > 0.8*num_iterations) current_iteration=0;
      if (ImGui::Button("Uniform Restart")) {
	current_iteration = 0;
	Initialize(ColdStart);
      }
      if (ImGui::Button("Random Restart")) {
	current_iteration = 0;
	Initialize(RandomizeFlip); 
      }


      ImGui::Separator();
      static bool ran_experiment = false;
      static bool running_experiment = false;
      static int current_trial= 0;
      static int skip_trials= 22;
      static int trials = 33;
      static int trial_each_temp = 10;
      static int current_trial_each_temp = 0;
      static int current_configuration = 0;
      static int n = 500;
      static int warm_up_sweeps = 300;

      static std::vector<float> energies(n-warm_up_sweeps);
      static std::vector<float> magnetizations(n-warm_up_sweeps);

      static std::vector<float> magnetization_trials(trials);
      static std::vector<float> energy_trials(trials);
      static float max_energy{0.5f};
      static float min_energy{0.0f};
      static float max_susc{0.5f};
      static float min_susc{0.f};


      
      if (ImGui::Button("Run Experiment.")) {
	ran_experiment = true;
	running_experiment = true;
	std::fill(std::begin(magnetization_trials), std::end(magnetization_trials), 0.f);
	std::fill(std::begin(energy_trials), std::end(energy_trials), 0.f);
	current_trial = skip_trials-1;
      }
      ImGui::Text("Experiment details:"); 
      ImGui::Text("Temperature steps %d of size 0.1", trials);
      ImGui::Text("   starting from:T= %f", 0.1f*skip_trials);
      ImGui::Text("Trials each Temperature:%d", trial_each_temp);
      ImGui::Text("Sweeps: %d", n); 
      ImGui::Text("  of which are warm-up: %d", warm_up_sweeps); 

      if(running_experiment && not pause_simulation){
        static float progress = 0.0f; 
	ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

	if(current_configuration >= warm_up_sweeps){
	  energies[current_configuration-warm_up_sweeps] = EnergyDensity();
	  magnetizations[current_configuration-warm_up_sweeps] = MeanMagnetization();
	}
	current_configuration++;
	temp = 0.1*current_trial+0.05f;
	if(current_configuration == n) {
	  progress += 1.f / (trial_each_temp*(trials-skip_trials));
	  current_configuration = 0;
	  magnetization_trials[current_trial] += Var(magnetizations);
	  energy_trials[current_trial] += Var(energies);
	  current_iteration = 0;
	  Initialize(RandomizeFlip);
	  current_trial_each_temp++;
	}
	if(current_trial_each_temp == trial_each_temp){
	  current_trial_each_temp = 0;
	  current_trial++;
	}	
	if(current_trial == trials) {
	  running_experiment = false;
	  current_trial = skip_trials-1;
	  progress = 0.f;
	  using std::begin; using std::end;
	  std::transform(begin(magnetization_trials), end(magnetization_trials), begin(magnetization_trials),
			 [](float x) { return x/trial_each_temp; });
	  std::transform(begin(energy_trials), end(energy_trials), begin(energy_trials),
			 [](float x) { return x/trial_each_temp; });
	  max_energy = *std::max_element(std::begin(energy_trials)+skip_trials, std::end(energy_trials));
	  min_energy = *std::min_element(std::begin(energy_trials)+skip_trials, std::end(energy_trials));
	  max_susc= *std::max_element(std::begin(magnetization_trials)+skip_trials, std::end(magnetization_trials));
	  min_susc= *std::min_element(std::begin(magnetization_trials)+skip_trials, std::end(magnetization_trials));
	} 
      }


      if(ran_experiment){
	static float(*susc)(void*,int) = [](void*, int i) ->float { return magnetization_trials[i]; };
	static float(*spec)(void*,int) = [](void*, int i) ->float { return energy_trials[i]; };
	ImGui::PlotHistogram("Susceptibility", susc, NULL, trials, 0, NULL, min_susc, max_susc , ImVec2(0, 80));
	ImGui::PlotHistogram("Specific Heat", spec, NULL, trials, 0, NULL, min_energy, max_energy, ImVec2(0, 80));
      }
 

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

      ImGui::SliderFloat("Point Size", &point_size, 0.5f, 50.0f); 
      ImGui::End();
      //GUI::Demo();
    };

  window.AddDrawCall(DrawPoints);
  window.AddDrawCall(DrawGui);
  auto RenderUpdate = [&window, &UpdateBuffer]()
    {
      do{
	window.UpdateEvents();
	UpdateBuffer();
	window.Update();
	if(window.ShouldClose()) { window.Exit(); return true; }
      } while(pause_simulation);
      return false;
    };
  Run(RenderUpdate);
}
