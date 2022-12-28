#include "Compressor.hpp"

vector<int> Compressor::compress(string data)
{
	vector<int> compressed(0);
	unordered_map<string, int> table;

	//init
	for (int i = 0; i < 256; i++)
		table[string(1, i)] = i;

	size_t lastInd = 255;
	size_t currInd = 0, nextInd = 1;

	string current_char = string(1, data[currInd]), next_char = string(1, data[nextInd]);
	int tmp = 0, toMove = 0;
	while (nextInd < data.length())
	{
		if (table[current_char] > 0)
		{
			toMove++;
			tmp = table[current_char];
			current_char = current_char + next_char;
			next_char = data[++nextInd];
		}
		else
		{
			compressed.push_back(tmp);
			table[current_char] = ++lastInd;
			currInd += toMove;
			current_char = data[currInd];
			toMove = 0;
		}
	}

	if (table[current_char] > 0)
		compressed.push_back(table[current_char]);
	else
	{
		compressed.push_back(tmp);
		compressed.push_back(current_char.back());
	}

	return compressed;
}

string Compressor::decompress(vector<int>& data)
{
	unordered_map<int, string> dict;
	string res;

	//initialize
	for (int i = 0; i < 256; ++i)
		dict[i] = char(i);

	int next = 1, lastInd = 256, tmp = data[0];

	string str = dict[tmp];
	string curr = string(1, str[0]);

	res.append(str);

	//decr
	for (int i = 0; i < data.size() - 1; i++)
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

		res.append(str);

		curr = "";
		curr += str[0];
		dict[lastInd] = dict[tmp] + curr;
		lastInd++;
		tmp = next;
	}

	return res;
}