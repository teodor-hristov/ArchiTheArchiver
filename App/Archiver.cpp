#include "Archiver.hpp"

#include <filesystem>
#include <iostream>
#include <stack>

#include "Compressor.hpp"

FileEntry::FileEntry(const string fileName) : data_size_compressed(0), data_size_raw(0)
{
	memset(file_name, 0, sizeof(file_name));
	for (int i = 0; i < fileName.length(); i++)
		file_name[i] = fileName.c_str()[i];
}

Archiver::Archiver() : archivedFilePath(""), archive(vector<FileEntry*>()) {}

Archiver::~Archiver()
{
	for(const auto& e : archive)
		delete e;
}

void Archiver::zip(string saveLocation, vector<string>& files) {
	archivedFilePath = saveLocation;

	for (auto& f : files)
	{
		try {
			if (std::filesystem::is_directory(f))
				addDir(f);
			else
				addFile(f);
		}
		catch (std::exception& e)
		{
			std::cerr << "File not added! " << e.what() << endl;
		}
	}

	saveToLocation(saveLocation);
}

void Archiver::editFile(string compressedFilePath, string newFilePath)
{
	FileEntry newFileEntry(newFilePath);
	fstream newFile(newFilePath, ios_base::in | ios_base::binary);
	fstream tmpCompressed(newFilePath + ".tmp", ios_base::binary
		| ios_base::out | ios_base::trunc);

	if (!newFile.is_open() || !tmpCompressed.is_open())
	{
		newFile.close();
		tmpCompressed.close();
		throw std::bad_exception();
	}

	Compressor::compress(newFile, tmpCompressed, &newFileEntry);

	size_t newFileSize = tmpCompressed.tellg();
	tmpCompressed.close();

	FileEntry archiveEntry("");
	fstream archive(compressedFilePath, ios_base::binary | ios_base::in);
	string newFileName = filesystem::path(newFilePath).filename().string();

	if (!archive.is_open())
		throw std::bad_exception();
	else
		archivedFilePath = compressedFilePath;

	//find file in archive
	while (archive.read(reinterpret_cast<char*>(&archiveEntry), sizeof(FileEntry)).gcount() > 0 &&
		filesystem::path(archiveEntry.file_name).filename().string() !=
		filesystem::path(newFilePath).filename().string())
	{
		if (!archive.seekg(archiveEntry.data_size_compressed, ios_base::cur).good())
			break;
	}

	streamsize fileEntryOffset = archive.seekg((-1) * sizeof(FileEntry), ios_base::cur).tellg();
	archive.close();

	size_t oldFileSize = archiveEntry.data_size_compressed;

	shiftFileContent(compressedFilePath, fileEntryOffset, newFileSize - oldFileSize);

	tmpCompressed.open(newFilePath + ".tmp", ios_base::binary | ios_base::in);
	archive.open(compressedFilePath, ios_base::binary | ios_base::out | ios_base::in);
	if (!archive.is_open())
		throw std::bad_exception();

	archive.seekp(fileEntryOffset, ios_base::beg);
	archive.write(reinterpret_cast<char*>(&newFileEntry), sizeof(FileEntry));

	//cpy tmp file
	char buf[4096] = { 0 };
	while (tmpCompressed.read(buf, sizeof(buf)).gcount() > 0)
	{
		//cout << tmpCompressed.gcount() << endl;
		archive.write(buf, tmpCompressed.gcount());
	}
	archive.flush();
	archive.close();

	tmpCompressed.close();
	if (std::filesystem::remove(compressedFilePath + ".tmp"))
		cerr << "Failed to delete .tmp file." << endl;

	newFile.close();
}

string Archiver::info(string& compressedFileLocation) const
{
	stringstream inf;
	FileEntry tempEntry("");

	fstream compressed(compressedFileLocation, ios_base::in | ios_base::binary);
	if (!compressed.is_open())
		throw std::invalid_argument("File not opening.");

	inf << "\n==============================\n";
	while (inf << compressed.tellg() << endl && compressed.read(reinterpret_cast<char*>(&tempEntry), sizeof(tempEntry)).gcount() > 0)
	{
		inf << "File name: " << tempEntry.file_name << endl;
		inf << "Compressed size: " << tempEntry.data_size_compressed << endl;
		inf << "Raw size: " << tempEntry.data_size_raw << endl;

		compressed.seekg(tempEntry.data_size_compressed, ios_base::cur);
	}
	inf << "==============================\n";

	compressed.close();
	return inf.str();
}

void Archiver::saveToLocation(string& locationPath) const
{
	if (std::filesystem::is_directory(locationPath))
		throw std::exception("Invalid file.");

	fstream compressedFile(locationPath, ios_base::out | ios_base::binary | ios_base::trunc);
	if (!compressedFile.is_open())
		throw std::exception("Cannot open file.");

	fstream in;
	for (FileEntry* fe : archive)
	{
		if (std::filesystem::is_directory(fe->file_name))
		{
			fe->data_size_compressed = 0;
			fe->data_size_raw = 0;
			compressedFile.write(reinterpret_cast<const char*>(fe), sizeof(FileEntry));
			cout << "Dir: " << fe->file_name << fe->data_size_compressed << " Offset: " << compressedFile.tellp() << endl;
			continue;
		}

		in.open(fe->file_name, ios_base::binary | ios_base::in);
		if (in.is_open())
		{
			compressedFile.write(reinterpret_cast<const char*>(fe), sizeof(FileEntry));
			Compressor::compress(in, compressedFile, fe);
			cout << "Name: " << fe->file_name << " Raw: " << fe->data_size_raw << " Compr: " << fe->data_size_compressed << " Offset: " << compressedFile.tellp() << endl;

			const streamsize pos = compressedFile.tellp();
			if (pos > 0) {
				compressedFile.seekp(pos - fe->data_size_compressed - sizeof(FileEntry), ios_base::beg); //move back
				compressedFile.write(reinterpret_cast<const char*>(fe), sizeof(FileEntry));
				compressedFile.seekp(0, ios_base::end); //move back
			}
		}

		in.close();
	}

	compressedFile.flush();
	compressedFile.close();
}

void Archiver::addFile(string& filePath)
{
	if (filePath != archivedFilePath)
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
		{
			for (const auto& e : filesystem::directory_iterator(curr))
				front.push(e.path().string());
		}

		addFile(curr);
		curr = front.top();
		front.pop();
	}
}
