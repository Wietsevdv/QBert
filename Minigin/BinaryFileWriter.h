#pragma once
#include <fstream>

namespace dae
{
	class BinaryFileWriter
	{
	public:
		BinaryFileWriter(const std::string& file);

		~BinaryFileWriter();
		BinaryFileWriter(const BinaryFileWriter& other) = delete;
		BinaryFileWriter(BinaryFileWriter&& other) = delete;
		BinaryFileWriter operator=(const BinaryFileWriter& other) = delete;
		BinaryFileWriter operator=(BinaryFileWriter&& other) = delete;

		void Write(const char* pData, std::streamsize count);

	private:
		std::ofstream m_File;
	};

}