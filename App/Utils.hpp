#include <vector>
#include <string>

struct FileHeader
	{
		double compression_level;
		size_t data_size;
		std::string file_name;
		//uint32_t file_hash;
	};
struct FileEntry
	{
		FileHeader header;
		std::vector<int> data;

		FileEntry(std::string fileName);
};