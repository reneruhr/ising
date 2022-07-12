#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../tools/tools.h"
#include <functional>
#include <iostream>
#include <vector>

namespace Render
{

auto SquareBuffer() ->	std::function<void(void)>;

enum class DrawMethod : GLenum {
  Static = GL_STATIC_DRAW, Dynamic = GL_DYNAMIC_DRAW };
  
struct Attribute
{
  std::size_t offset_;
  std::size_t size_;
  std::size_t dim_;
};
  
class Buffer
{
private:
  unsigned int vao_, vbo_, ebo_;
  std::vector<Attribute> attributes_;
  std::size_t size_;

  template <class C>
  void AddAttribute(const C& container)
  {
    using vector = typename C::value_type;
    auto dim = sizeof(vector)/sizeof(float);		
    auto size = std::size(container) * sizeof(vector);
    size_ = std::size(container);
    auto offset = std::size(attributes_) ? attributes_.back().offset_ + attributes_.back().size_  : 0;

    std::cout << "Adding Attribute (dim,size,offset)=" << dim  << ',' << size << ',' << offset <<'\n'; 
    std::cout << "First/Last value= " << container[0] << container.back() << '\n';
    glBufferSubData(GL_ARRAY_BUFFER, offset,
		    size,
		    container.data()); 
    glEnableVertexAttribArray(std::size(attributes_));
    glVertexAttribPointer(std::size(attributes_), dim, GL_FLOAT, GL_FALSE,
			  sizeof(vector), (void*)(offset));
    attributes_.emplace_back(offset, size, dim);
  } 


public:
  template <class ...C>
  Buffer(DrawMethod method, const C& ...containers)
  {	
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, Tools::TotalContainersSize(containers...), NULL, static_cast<GLenum>(method));
   (..., AddAttribute(containers));
    glBindVertexArray(0);
  }

  template <class C>
  void Update(const C& container, std::size_t index)
  {
    auto size = attributes_[index].size_; 
    auto offset =  attributes_[index].offset_;

    glBufferSubData(GL_ARRAY_BUFFER, offset,
		    size,
		    container.data()); 
  }
  std::size_t Size() const { return size_; }
  unsigned int Vao() const { return vao_; }
};

  auto PointsBuffer(const Buffer&) -> std::function<void(void)>;

}
