
typedef struct FileHeader_Tag
{
	double compression_level;
	size_t data_size;
	string file_name;

	FileHeader(string fileName, size_t dataSize, double compressionLevel) :
		file_name(fileName), dataSize(data_size), compression_level(compression_level){}
} FileHeader;

class Archiver {


private:
	//[(archive info)-(file header)(file data) ... (file header)(file data)]

public:
	virtual void saveToLocation(string& locationPath)
	virtual void addFile(string& filePath) = delete;
	virtual void addDir(string& dirPath) = delete;
	virtual double compressionLevel() const = delete;
	virtual string info() const = delete;
};
