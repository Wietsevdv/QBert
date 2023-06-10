#include "BinaryFileWriter.h"

dae::BinaryFileWriter::BinaryFileWriter(const std::string& file)
{
	m_File.open(file, std::ios::binary);
}

dae::BinaryFileWriter::~BinaryFileWriter()
{
	m_File.close();
}

void dae::BinaryFileWriter::Write(const char* pData, std::streamsize count)
{
	m_File.write(pData, count);
}
