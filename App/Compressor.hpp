#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Compressor
{
public:
	virtual vector<int> compress(string data);
	virtual string decompress(vector<int>& data);
};
