#ifndef PIPEY_POSIXTIMEHELPER_H
#define PIPEY_POSIXTIMEHELPER_H

#include <time.h>

namespace pipey {
	namespace util {
		void RelativeToAbsolute(unsigned long nMilliseconds, timespec &rAbsolute);
	}
}

#endif
