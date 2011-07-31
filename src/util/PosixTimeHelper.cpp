#include "PosixTimeHelper.h"
#include <sys/time.h>

namespace pipey {
	namespace util {

		void RelativeToAbsolute(unsigned long nMilliseconds, timespec *pAbsolute)
		{
			timeval now;
			gettimeofday(&now, NULL);

			long milli = (nMilliseconds%1000) + (now.tv_usec/1000);

			pAbsolute->tv_sec = milli/1000 + now.tv_sec + nMilliseconds/1000;
			pAbsolute->tv_nsec = (milli%1000) * 1000 * 1000;
		}

	}
}
