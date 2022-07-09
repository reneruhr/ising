#pragma once

#include <array>
#include <span>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Render
{

struct Camera
{
	using OrthFrame = std::array<float, 6>;

	glm::mat4 view_;
	glm::mat4 projection_;	
	glm::mat4 pv_;
	
	glm::vec3 front_ = {0.0f, 0.0f, -0.5f};
	glm::vec3 pos_ = {0.0f, 0.0f, 0.5f};
	glm::vec3 up_ = {0.0f, 1.0f, 0.0f};

	OrthFrame ortho_frame_;

	Camera() { View(); Projection({-1.f,1.f,-1.f,1.f,-1.f,1.f}); }	
	
	auto PV() -> std::span<float,16>
	{
		return std::span<float,16>(glm::value_ptr(pv_), 16);
	}

	auto View() -> glm::mat4&
	{
		view_ = glm::lookAt(pos_, pos_ + front_, up_);	
		pv_ =  projection_ * view_;
		return view_; 
	}
	
	auto Projection(OrthFrame frame) -> glm::mat4&
	{
		ortho_frame_ = frame;
		projection_ = glm::ortho(frame[0], frame[1], 
				  frame[2], frame[3],
				  frame[4], frame[5]);
		pv_ =  projection_ * view_;
		return projection_;
	}

};
}

