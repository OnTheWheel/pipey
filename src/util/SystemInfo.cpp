#include "SystemInfo.h"

#if defined(__linux__) || defined(__unix__)
#include <unistd.h>
#endif

namespace pipey {
	namespace util {

#if defined(WIN32) || defined(WIN64)

		uint32_t GetNumberOfProcessors()
		{
			SYSTEM_INFO tSysInfo;
			GetSystemInfo(&tSysInfo);

			return tSysInfo.dwNumberOfProcessors ;
		}
#elif defined(__linux__) || defined(__unix__)

		uint32_t GetNumberOfProcessors()
		{
			return (uint32_t)sysconf(_SC_NPROCESSORS_ONLN);
		}
#endif

	}
}
