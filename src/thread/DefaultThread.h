#ifndef PIPEY_DEFAULT_THREAD_H
#define PIPEY_DEFAULT_THREAD_H

#if defined(WIN32) || defined(WIN64)
#include "WindowsThread.h"
#elif defined(__linux__) || defined(__unix__)
#include "PosixThread.h"
#endif

namespace pipey {
	namespace thread {
#if defined(WIN32) || defined(WIN64)
		typedef CWindowsThread CDefaultThread;
#elif defined(__linux__) || defined(__unix__)
		typedef CPosixThread CDefaultThread;
#endif
	}
}

#endif