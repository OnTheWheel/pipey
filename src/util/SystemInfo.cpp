#include "SystemInfo.h"

namespace pipey {
	namespace util {

#if defined(WIN32) || defined(WIN64)

		unsigned long GetNumberOfProcessors()
		{
			SYSTEM_INFO tSysInfo;
			GetSystemInfo(&tSysInfo);

			return tSysInfo.dwNumberOfProcessors ;
		}

#endif

	}
}