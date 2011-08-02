#include "PosixTimeHelper.h"
#include <sys/time.h>

namespace pipey {
	namespace util {

		void RelativeToAbsolute(unsigned long nMilliseconds, timespec &rAbsolute)
		{
			timeval now;
			gettimeofday(&now, NULL);

			long milli = (nMilliseconds%1000) + (now.tv_usec/1000);

			rAbsolute.tv_sec = milli/1000 + now.tv_sec + nMilliseconds/1000;
			rAbsolute.tv_nsec = (milli%1000) * 1000 * 1000;
		}

	}
}
