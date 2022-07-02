#pragma once
#include <string>

namespace Tools
{
auto LoadFile(std::string)  -> std::string;

template<typename C>
std::size_t ContainerSize(const C& container)
{
  return std::size(container) * sizeof(typename C::value_type);
}

template<typename... Containers>
std::size_t TotalContainersSize(const Containers& ...container)
{
  return  (ContainerSize(container) + ... + 0);
}
}
