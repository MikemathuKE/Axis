#pragma once

#include <string>

namespace Axis {

	enum FileType {
		AxisScene
	};

	class FileDialogs
	{
	public:
		static std::string OpenFile(FileType type);
		static std::string SaveFile(FileType type);
	private:
		static const char* FileTypeToString(FileType type);
	};

}