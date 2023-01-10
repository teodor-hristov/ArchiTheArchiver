#pragma once

#include <string>

struct FileEntry
	{
		size_t data_size_compressed; //in bytes
		size_t data_size_raw; //in bytes
		char file_name[256];
		//uint32_t file_hash;

		FileEntry(const std::string fileName);
};

void shiftFileContent(const std::string& file_name, std::streampos pointer, std::streampos shift);