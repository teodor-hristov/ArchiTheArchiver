#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Utils.hpp"

using namespace std;

class Compressor
{
public:
	virtual void compress(istream& in, ostream& out, FileEntry* fe);
	void decompress(istream& in, ostream& out, FileEntry* fe);
};
