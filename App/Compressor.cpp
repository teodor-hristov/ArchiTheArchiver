#include "Compressor.hpp"

#include <algorithm>
#include <unordered_map>
#include <string>
#include <iostream>

using std::istream, std::ostream, std::unordered_map, std::string, std::ios_base;

void Compressor::compress(istream& in, ostream& out, FileEntry* fe)
{
	if (!fe)
		throw std::exception("File entry null.");

	if (!in.good() || !out.good())
		throw std::exception("Stream failure.");

	constexpr int maxDictSize = 4096;
	unordered_map<string, uint16_t> table(maxDictSize);

	string currentString;
	string nextString = "";

	int dictSize = 256;
	char unit = 0;

	for (int i = 0; i < dictSize; i++)
		table[string(1, i)] = i;

	in.read(&unit, sizeof(unit));
	currentString = unit;

	string newElement;
	while (in.read(&unit, sizeof(unit)).gcount() > 0 && unit != '\0')
	{
		nextString = unit;
		newElement = currentString + nextString;
		if (table.contains(newElement))
		{
			currentString += nextString;
		}
		else
		{
			//cout << table[currentString] << ", ";
			out.write(reinterpret_cast<const char*>(&table[currentString]), sizeof(table[currentString]));
			fe->data_size_compressed += sizeof(table[currentString]);
			fe->data_size_raw += currentString.length();

			if (maxDictSize > dictSize)
			{
				table[newElement] = dictSize;
				dictSize++;
			}

			currentString = nextString;
		}

	}

	//cout << table[currentString] << endl;
	out.write(reinterpret_cast<const char*>(&table[currentString]), sizeof(table[currentString]));
	fe->data_size_compressed += sizeof(table[currentString]);
	fe->data_size_raw += currentString.length();

	out.flush();
}

void Compressor::decompress(istream& in, ostream& out, const FileEntry* fe)
{
	if (!fe)
		throw std::exception("Bad FileEntry.");

	if (!in.good() || !out.good())
		throw std::exception("Bad streams.");

	unordered_map<uint16_t, string> dict;

	//initialize
	for (int i = 0; i < 256; ++i)
		dict[i] = static_cast<char>(i);

	uint16_t data[2048] = { 0 };
	int next, tmp = 0;
	int lastInd = 256;

	const size_t end_offs = static_cast<int>(in.tellg()) + fe->data_size_compressed;
	size_t tmpSize = fe->data_size_compressed;
	string str, curr;

	//Decrypt
	int red = in.read(reinterpret_cast<char*>(data), sizeof(data)).gcount();
	while (in.gcount() > 0 && in.tellg() < end_offs)
	{
		str = dict[data[tmp]];
		curr = string(1, str[0]);
		out.write(str.c_str(), str.length());

		for (int i = 0; i < std::ranges::min(static_cast<int>(tmpSize), red)/2 - 1; i++)
		{
			next = data[i + 1];
			if (dict.find(next) == dict.end())
			{
				str = dict[tmp];
				str += curr;
			}
			else
			{
				str = dict[next];
			}

			out.write(str.c_str(), str.length());

			curr = "";
			curr += str[0];
			dict[lastInd] = dict[tmp] + curr;
			lastInd++;
			tmp = next;
		}
		tmpSize -= in.read(reinterpret_cast<char*>(data), sizeof(data)).gcount();
	}

	in.clear();
	in.seekg(end_offs, ios_base::beg);
}
