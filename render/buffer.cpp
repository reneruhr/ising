#include "buffer.h"


namespace Render
{

auto SquareBuffer() -> std::function<void(void)> 
{
     auto square = std::array<float,8>
	  {
	       1.f,  1.f,
	       1.f, -1.f,
	       -1.f, -1.f, 
	       -1.f,  1.f 
	  }; 
	
     auto indices = std::array<unsigned int, 6>
	  {
	       0,1,3,1,2,3
	  };

     unsigned int vao, vbo, ebo;
     glGenVertexArrays(1, &vao);
     glGenBuffers(1, &vbo);
     glGenBuffers(1, &ebo);
	
     glBindVertexArray(vao);
     glBindBuffer(GL_ARRAY_BUFFER, vbo);
     glBufferData(GL_ARRAY_BUFFER, sizeof(square), square.data(), GL_STATIC_DRAW);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(0);
     glBindVertexArray(0);

     return [vao,vbo,ebo]()
	  {
	       glBindVertexArray(vao);
	       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	  };	
}

auto PointsBuffer(const Buffer& buffer) -> std::function<void(void)>
{
  return [vao = buffer.Vao(), size = buffer.Size() ]()
	  {
	       glBindVertexArray(vao);
	       glDrawArrays(GL_POINTS, 0, size);
	  };
}

}
