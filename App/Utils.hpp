#pragma once

#include <vector>
#include <string>

struct FileEntry
	{
		size_t data_size_compressed; //in bytes
		size_t data_size_raw; //in bytes
		char file_name[32];
		//uint32_t file_hash;

		FileEntry(std::string& fileName);
};