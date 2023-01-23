#pragma once

#include "Utils.hpp"

namespace Compressor
{
	void compress(std::istream& in, std::ostream& out, FileEntry* fe);
	void decompress(std::istream& in, std::ostream& out, const FileEntry* fe);
}
