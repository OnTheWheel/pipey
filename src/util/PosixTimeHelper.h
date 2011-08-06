#ifndef PIPEY_POSIXTIMEHELPER_H
#define PIPEY_POSIXTIMEHELPER_H

#include <time.h>
#include <sys/time.h>

namespace pipey {
	namespace util {

		struct timeval_ext : public timeval
		{
			unsigned long to_milli_seconds();
		};

		void RelativeToAbsolute(unsigned long nMilliseconds, timespec &rAbsolute);

		unsigned long GetTimeMilliSeconds();
	}
}

#endif
