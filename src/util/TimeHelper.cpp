#include "TimeHelper.h"

#if defined(__linux__) || defined(__unix__)
#include <unistd.h>
#endif

namespace pipey {
	namespace util {

#if defined(WIN32) || defined(WIN64)

		void SleepSeconds(uint32_t nSeconds)
		{
			::Sleep( nSeconds * 1000 );
		}
#elif defined(__linux__) || defined(__unix__)

		void SleepSeconds(uint32_t nSeconds)
		{
			sleep(nSeconds);
		}
#endif

	}
}
