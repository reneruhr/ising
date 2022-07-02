#include "colors.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Render
{
auto MakeRGB(Color color) -> Vec3
{
	static glm::vec3 Green{0.f,1.f,0.f};
	static glm::vec3 Black{0.f,0.f,0.f};
	

	static glm::vec3 Salmon = glm::vec3(250,128,114)/255.f;
	static glm::vec3 ForestGreen= glm::vec3(34,139,34)/255.f;
	static glm::vec3 LightSeaGreen= glm::vec3(32,178,170)/255.f;
	static glm::vec3 RoyalBlue= glm::vec3(65,105,225)/255.f;
	static glm::vec3 HotPink= glm::vec3(255,105,180)/255.f;
	static glm::vec3 Chocolate= glm::vec3(210,105,30)/255.f;
	static glm::vec3 Ivory= glm::vec3(255,255,240) /255.f;
	static glm::vec3 Silver= glm::vec3(192,192,192)/255.f;
	static glm::vec3 Honeydew= glm::vec3(240,255,240)/255.f;
	static glm::vec3 CornFlowerBlue= glm::vec3(100,149,237)/255.f;
	static glm::vec3 DarkTurqoise= glm::vec3(0,206,209)/255.f;
	static glm::vec3 MediumSpringGreen= glm::vec3(0,250,154)/255.f;
	static glm::vec3 Tomato = glm::vec3(255,99,71)/255.f;
	static glm::vec3 LemonChiffon = glm::vec3(255,250,205)/255.f;

switch(color){
	case(Color::Green): return Vec3(glm::value_ptr(Green), 3);
	case(Color::ForestGreen): return Vec3(glm::value_ptr(ForestGreen), 3);
	case(Color::LightSeaGreen): return Vec3(glm::value_ptr(LightSeaGreen), 3);
	case(Color::RoyalBlue): return Vec3(glm::value_ptr(RoyalBlue), 3);
	case(Color::HotPink): return Vec3(glm::value_ptr(HotPink), 3);
	case(Color::Chocolate): return Vec3(glm::value_ptr(Chocolate), 3);
	case(Color::Ivory): return Vec3(glm::value_ptr(Ivory), 3);
	case(Color::Silver): return Vec3(glm::value_ptr(Silver), 3);
	case(Color::Honeydew): return Vec3(glm::value_ptr(Honeydew), 3);
	case(Color::CornFlowerBlue): return Vec3(glm::value_ptr(CornFlowerBlue), 3);
	case(Color::DarkTurqoise): return Vec3(glm::value_ptr(DarkTurqoise), 3);
	case(Color::MediumSpringGreen): return Vec3(glm::value_ptr(MediumSpringGreen), 3);
		case(Color::Tomato): return Vec3(glm::value_ptr(Tomato), 3);
	case(Color::LemonChiffon): return Vec3(glm::value_ptr(Green), 3);
	default:   return  Vec3(glm::value_ptr(Black),3);
} 
}

auto NextColor() -> Color { 
	 static int i = 0;
	 static const int count = static_cast<std::underlying_type_t<Color>>(Color::COUNT); 
	 if(i == count) i=0;
	 return static_cast<Color>(i++);
}
}
