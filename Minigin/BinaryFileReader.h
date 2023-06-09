#pragma once
#include <fstream>

namespace dae
{
	class BinaryFileReader final
	{
	public:
		BinaryFileReader(const std::string& file);

		~BinaryFileReader();
		BinaryFileReader(const BinaryFileReader& other) = delete;
		BinaryFileReader(BinaryFileReader&& other) = delete;
		BinaryFileReader operator=(const BinaryFileReader& other) = delete;
		BinaryFileReader operator=(BinaryFileReader&& other) = delete;

		char* Read(std::streamsize count);

	private:
		std::ifstream m_File;
	};

}