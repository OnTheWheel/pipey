#ifndef PIPEY_DEFAULTTIMER_H
#define PIPEY_DEFAULTTIMER_H

#if defined(WIN32) || defined(WIN64)
#include "WindowsTimer.h"
#elif defined(__linux__) || defined(__unix__)
#include "PosixTimer.h"
#endif

namespace pipey {
	namespace util {
#if defined(WIN32) || defined(WIN64)
		typedef CWindowsTimer CDefaultTimer;
#elif defined(__linux__) || defined(__unix__)
		typedef CPosixTimer CDefaultTimer;
#endif
	}
}

#endif