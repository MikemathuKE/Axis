
#include "axispch.h"

#include "Axis/utils/PlatformUtils.h"

#ifdef AXIS_PLATFORM_LINUX

#include "nfd.h"
#include <fstream>

namespace Axis {

    const char* FileDialogs::FileTypeToString(FileType type)
    {
        switch (type)
        {
        case Axis::AxisScene:
            return "axis\0";
        default:
            return "\0";
        }
    }

	std::string FileDialogs::OpenFile(FileType type)
	{
        const char* filter = FileTypeToString(type);

        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog( filter, NULL, &outPath );
        if ( result == NFD_OKAY )
        {
            std::string filePath(outPath);
            free(outPath);
            return filePath;
        }
        // result == NFD_CANCEL or error occured Print using NFD_GETError()
        return std::string();
	}

	std::string FileDialogs::SaveFile(FileType type)
	{
        const char* filter = FileTypeToString(type);

        nfdchar_t *savePath = NULL;
        nfdresult_t result = NFD_SaveDialog( filter, NULL, &savePath );
        if ( result == NFD_OKAY )
        {
            std::string filePath(savePath);
            free(savePath);
            return filePath;
        }
        return std::string();
	}

}

#endif // AXIS_PLATFORM_LINUX

