#include "Archiver.hpp"

#include <filesystem>
#include <iostream>
#include <stack>

#include "Compressor.hpp"

FileEntry::FileEntry(string fileName)
{
	header = FileHeader{ 0,0, fileName};
}

void Archiver::prepareForSave()
{

}

Archiver::Archiver()
{
	archive = vector<FileEntry*>();
}

void Archiver::saveToLocation(string& locationPath)
{
	const string saveTo = locationPath + "compressedFile.archi";
	const fstream file(saveTo, ios_base::out | ios_base::binary | ios_base::trunc);

	if (!file.is_open())
		throw std::exception("Cannot open file.");



}

void Archiver::addFile(string& filePath)
{
	const fstream file(filePath, ios_base::in);
	if (!file.is_open())
		throw std::exception("Cannot open file.");

	archive.emplace_back(new FileEntry(filePath));
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
