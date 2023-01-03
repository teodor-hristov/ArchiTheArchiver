#include "Compressor.hpp"

#include <fstream>
#include <iostream>

void Compressor::compress(istream& in, ostream& out, FileEntry* fe)
{
	if (!fe)
		throw std::exception("File entry null.");

	if (!in.good() || !out.good())
		throw std::exception("Stream failure.");

	unordered_map<string, uint16_t> table;
	char data[4096] = { 0 };
	std::streamsize dataSize;

	//init
	for (int i = 0; i < 256; i++)
		table[string(1, i)] = i;

	size_t lastInd = 255;
	size_t currInd = 0, nextInd = 1;

	string current_char;
	string next_char;
	uint16_t last_found = 0, toMove = 0;

	while (in.get(data, sizeof(data)))
	{
		current_char = data[currInd];
		next_char = data[nextInd];
		dataSize = in.gcount();
		fe->data_size_raw += dataSize;

		while (nextInd < dataSize)
		{
			if (table[current_char] > 0)
			{
				toMove++;
				last_found = table[current_char];
				current_char = current_char + next_char;
				next_char = data[++nextInd];
			}
			else
			{
				out.write(reinterpret_cast<const char*>(&last_found), sizeof(last_found));
				fe->data_size_compressed += sizeof(last_found);
				table[current_char] = ++lastInd;
				currInd += toMove;
				current_char = data[currInd];
				toMove = 0;
			}
		}

		if (table[current_char] > 0)
		{
			out.write(reinterpret_cast<const char*>(&table[current_char]), sizeof(table[current_char]));
			fe->data_size_compressed += 1 * sizeof(table[current_char]);
		}
		else
		{
			out.write(reinterpret_cast<const char*>(&last_found), sizeof(last_found));
			out.write(&current_char.back(), sizeof(current_char.back()));
			fe->data_size_compressed += sizeof(last_found) + sizeof(current_char.back());
		}

		//currInd = 0, nextInd = 1;
		//tmp = 0, toMove = 0;
	}
	out.flush();
}

void Compressor::decompress(istream& in, ostream& out, FileEntry* fe)
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
	int next, tmp;
	int lastInd = 256;

	int end_offs = static_cast<int>(in.tellg()) + fe->data_size_compressed;

	string str, curr;

	//Decrypt
	streamsize red = in.read(reinterpret_cast<char*>(data), sizeof(data)).gcount();
	while (red <= end_offs && red > 0)
	{
		tmp = data[0];
		str = dict[tmp];
		curr = string(1, str[0]);
		out.write(str.c_str(), str.length());

		for (int i = 0; i < red - 1; i++)
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

		red = in.read(reinterpret_cast<char*>(data), sizeof(data)).gcount();
	}
}
