#include "SystemInfo.h"

#if defined(__linux__) || defined(__unix__)
#include <sys/sysinfo.h>
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
			struct sysinfo tSysInfo;
			sysinfo(&tSysInfo);
			return (unsigned long)tSysInfo.procs;
		}
#endif

	}
}
