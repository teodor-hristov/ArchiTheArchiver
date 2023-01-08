#pragma once

#include "Utils.hpp"
#include <fstream>

using namespace std;

class Archiver {
private:
	void saveToLocation(string& locationPath) const;
	void addFile(string& filePath);
	void addDir(string& dirPath);

public:
	vector<FileEntry*> archive;
	Archiver();
	virtual void zip(string saveLocation, vector<string>& files);

	virtual void editFile(string compressedFile, string& newVersion);
	virtual string info(string& compressedFileLocation) const;
};
