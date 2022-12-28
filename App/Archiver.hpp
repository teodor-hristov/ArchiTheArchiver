#include <vector>
#include <string>
#include <fstream>

using namespace std;

//[(archive info)-(file header)(file data) ... (file header)(file data)]
struct FileHeader
	{
		double compression_level;
		size_t data_size;
		string file_name;
		uint32_t file_hash;
	};
struct FileEntry
	{
		FileHeader header;
		vector<int> data;

		FileEntry(string fileName);
};

class Archiver {
private:
	vector<FileEntry> archive;
public:
	Archiver();
	virtual void saveToLocation(string& locationPath) = delete;
	virtual void addFile(string& filePath);
	virtual void addDir(string& dirPath);
	virtual void removeFile(string& filePath) = delete;
	virtual void editFile(string& filePath) = delete;
	virtual double compressionLevel() const = delete;
	virtual string info() const = delete;
};
