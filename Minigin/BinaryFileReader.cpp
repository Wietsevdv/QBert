#include "BinaryFileReader.h"

dae::BinaryFileReader::BinaryFileReader(const std::string& file)
{
	m_File.open(file, std::ios::binary);
}

dae::BinaryFileReader::~BinaryFileReader()
{
	m_File.close();
}

char* dae::BinaryFileReader::Read(char* dst, std::streamsize count)
{
	m_File.read(dst, count);

	if (m_File)
		return dst;

	return nullptr;
}
