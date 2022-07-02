#include "tools.h"
#include <fstream>
#include <sstream>
#include <string>

namespace Tools
{
auto LoadFile(std::string name) -> std::string
{
	auto in_file = std::ifstream(name);
	auto buffer = std::stringstream();
	buffer << in_file.rdbuf();
	return buffer.str();
}

}
