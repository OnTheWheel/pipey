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

		void SleepMilliseconds(uint32_t nMilliseconds)
		{
			::Sleep( nMilliseconds );
		}
#elif defined(__linux__) || defined(__unix__)

		void SleepSeconds(uint32_t nSeconds)
		{
			sleep(nSeconds);
		}

		void SleepMilliseconds(uint32_t nMilliseconds)
		{
			timeval tv;
			tv.tv_sec = nMilliseconds / 1000;
			tv.tv_usec = (nMilliseconds % 1000) * 1000;
			select(0, 0, 0, 0, &tv );
		}
#endif

	}
}
