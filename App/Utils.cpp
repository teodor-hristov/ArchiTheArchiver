#pragma once
#include "Utils.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

using std::streampos;

FileEntry::FileEntry() : FileEntry(""){}

void shiftFileContent(const std::string& file_name, std::streampos pointer, std::streampos shift) {
	std::ifstream input_file(file_name, std::ios::binary);
	std::ofstream output_file(file_name + ".tmp", std::ios::binary | std::ios_base::trunc);

	if (!input_file.is_open())
		throw std::system_error(EEXIST, std::generic_category());

	if (!output_file.is_open())
		throw std::system_error(EIO, std::generic_category());

	char buffer[1024];
	streampos red = pointer;

	// Read and write the bytes before the pointer
	while (input_file.read(buffer, sizeof(buffer)).gcount() > 0)
	{
		red -= input_file.gcount();
		if (red <= 0)
		{
			output_file.write(buffer,
				pointer + red < 0 ?
				pointer :
				pointer + red);
			break;
		}

		output_file.write(buffer, pointer);
	}

	// Shift the rest of the file by the given number of bytes
	input_file.seekg(pointer + shift);
	while (input_file.read(buffer, sizeof(buffer)).gcount() > 0)
	{
		output_file.write(buffer, input_file.gcount());
	}

	input_file.close();
	output_file.close();

	// Replace the original file with the modified version
	std::filesystem::remove(file_name.c_str());
	std::filesystem::rename((file_name + ".tmp").c_str(), file_name.c_str());
}
