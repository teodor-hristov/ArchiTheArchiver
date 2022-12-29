#include "Utils.hpp"
#include <fstream>

using namespace std;

class Archiver {
private:
	vector<FileEntry*> archive;

	void prepareForSave();
public:
	Archiver();
	virtual void saveToLocation(string& locationPath);
	virtual void addFile(string& filePath);
	virtual void addDir(string& dirPath);

	virtual void removeFile(string& filePath) = delete;
	virtual void editFile(string& filePath) = delete;

	virtual double compressionLevel() const = delete;
	virtual string info() const = delete;
};
