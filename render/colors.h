#pragma once
#include <span>

namespace Render
{
using Vec3= std::span<float,3>;
enum class Color
{
	Green,Black,Salmon,ForestGreen,LightSeaGreen,RoyalBlue,HotPink,Chocolate,Ivory,
	Silver,Honeydew,CornFlowerBlue,DarkTurqoise,MediumSpringGreen,Tomato,LemonChiffon,
	COUNT
};

auto MakeRGB(Color) -> Vec3;
auto NextColor() -> Color;
}
