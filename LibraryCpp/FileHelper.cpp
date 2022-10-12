#include "FileHelper.h"

std::string FileHelper::Read(std::ifstream* f)
{
	f->seekg(0, std::ios::end);
	size_t size = f->tellg();
	std::string buffer(size, ' ');
	f->seekg(0);
	f->read(&buffer[0], size);

	return buffer;
}
