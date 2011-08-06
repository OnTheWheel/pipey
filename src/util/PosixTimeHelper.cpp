#include "PosixTimeHelper.h"

namespace pipey {
	namespace util {
		
		unsigned long timeval_ext::to_milli_seconds()
		{
			return ((unsigned long)tv_sec)*1000 + ((unsigned long)tv_usec)/1000;
		}

		void RelativeToAbsolute(unsigned long nMilliseconds, timespec &rAbsolute)
		{
			timeval now;
			gettimeofday(&now, NULL);

			long milli = (nMilliseconds%1000) + (now.tv_usec/1000);

			rAbsolute.tv_sec = milli/1000 + now.tv_sec + nMilliseconds/1000;
			rAbsolute.tv_nsec = (milli%1000) * 1000 * 1000;
		}

		unsigned long GetTimeMilliSeconds()
		{
			timeval_ext now;
			gettimeofday(&now, NULL);
			return now.to_milli_seconds();
		}

	}
}
