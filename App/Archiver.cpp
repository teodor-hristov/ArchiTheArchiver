#include "Archiver.hpp"

#include <filesystem>
#include <iostream>
#include <stack>

#include "Compressor.hpp"

FileEntry::FileEntry(string fileName)
{
	header = FileHeader{ 0,0, fileName, 0 };
}

Archiver::Archiver()
{
	archive = vector<FileEntry>();
}

void Archiver::addFile(string& filePath)
{
	const fstream file(filePath, ios_base::in);
	if (!file.is_open())
		throw std::exception("Cannot open file.");

	archive.emplace_back(FileEntry(filePath));
}

void Archiver::addDir(string& dirPath)
{
	if (!filesystem::is_directory(dirPath))
		return;

	stack<string> front;
	string curr = dirPath;

	front.push(dirPath);
	while (!front.empty())
	{
		if (filesystem::is_directory(curr))
			for (const auto& e : filesystem::directory_iterator(curr))
				front.push(e.path().string());
		else
			addFile(curr);

		cout << curr << endl;
		curr = front.top();
		front.pop();
	}
}
