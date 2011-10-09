#include "SystemInfo.h"

#if defined(__linux__) || defined(__unix__)
#include <unistd.h>
#endif

namespace pipey {
	namespace util {

#if defined(WIN32) || defined(WIN64)

		unsigned long GetNumberOfProcessors()
		{
			SYSTEM_INFO tSysInfo;
			GetSystemInfo(&tSysInfo);

			return tSysInfo.dwNumberOfProcessors ;
		}
#elif defined(__linux__) || defined(__unix__)

		unsigned long GetNumberOfProcessors()
		{
			return (unsigned long)sysconf(_SC_NPROCESSORS_ONLN);
		}
#endif

	}
}
