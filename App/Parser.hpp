#pragma once

#include <iostream>
#include <vector>

namespace parser
{
	//bool zip(std::vector<std::string>& files);
	bool unzip(std::string& filePath);
	void parse(std::istream& in, std::ostream& out);
}
