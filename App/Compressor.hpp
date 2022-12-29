#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Utils.hpp"

using namespace std;

class Compressor
{
public:
	virtual void compress(istream& in, ostream& out);
	//virtual void compressFile(FileEntry* fe);
	virtual string decompress(vector<int>& data);
};
