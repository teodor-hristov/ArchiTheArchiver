#pragma once

#include "Utils.hpp"
#include <fstream>
#include <vector>

using namespace std;

class Archiver {
private:
	string archivedFilePath;

	void saveToLocation(const string& locationPath) const;
	void addFile(const string& filePath);
	void addDir(const string& dirPath);

public:
	vector<FileEntry*> archive;
	Archiver();
	virtual ~Archiver();
	virtual void zip(string saveLocation, vector<string>& files);
	virtual void editFile(string compressedFilePath, string newFilePath);
	virtual string info(string& compressedFileLocation) const;

	virtual void unzip(const string& archiveLocation, const string& unzipLocation);
};
