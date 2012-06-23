#ifndef PIPEY_POSIXTIMEHELPER_H
#define PIPEY_POSIXTIMEHELPER_H

#include <time.h>
#include <sys/time.h>
#include <stdint.h>

namespace pipey {
	namespace util {

		struct timeval_ext : public timeval
		{
			uint64_t to_milli_seconds();
		};

		void RelativeToAbsolute(uint32_t nMilliseconds, timespec &rAbsolute);

		uint64_t GetTimeMilliSeconds();
	}
}

#endif
