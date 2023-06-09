#include "BinaryFileReader.h"

dae::BinaryFileReader::BinaryFileReader(const std::string& file)
{
	m_File.open(file, std::ios::binary);
}

dae::BinaryFileReader::~BinaryFileReader()
{
	m_File.close();
}

char* dae::BinaryFileReader::Read(std::streamsize count)
{
	char* pData{ nullptr };
	m_File.read(pData, count);

	if (m_File)
		return pData;

	return nullptr;
}
