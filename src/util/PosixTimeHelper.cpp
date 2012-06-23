#include "PosixTimeHelper.h"

namespace pipey {
	namespace util {
		
		uint64_t timeval_ext::to_milli_seconds()
		{
			return tv_sec*1000 + tv_usec/1000;
		}

		void RelativeToAbsolute(uint32_t nMilliseconds, timespec &rAbsolute)
		{
			timeval now;
			gettimeofday(&now, NULL);

			uint64_t milli = (nMilliseconds%1000) + (now.tv_usec/1000);

			rAbsolute.tv_sec = milli/1000 + now.tv_sec + nMilliseconds/1000;
			rAbsolute.tv_nsec = (milli%1000) * 1000 * 1000;
		}

		uint64_t GetTimeMilliSeconds()
		{
			timeval_ext now;
			gettimeofday(&now, NULL);
			return now.to_milli_seconds();
		}

	}
}
