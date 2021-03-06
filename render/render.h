#pragma once

#include "window.h"
#include "gui.h"
#include "buffer.h"
#include "camera.h"
#include "colors.h"
#include <functional>
#include <span>
namespace Render 
{
using MVP = std::span<float,16>;
using Vec3= std::span<float,3>;

auto DrawTriangle() -> std::function<void(int, float)> ;
auto DrawPoints(const Buffer&) -> std::function<void(MVP , Color)> ;
auto DrawColoredPoints(const Buffer&) -> std::function<void(MVP, float)> ;

}
