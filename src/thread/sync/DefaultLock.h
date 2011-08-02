#ifndef PIPEY_DEFAULTLOCK_H
#define PIPEY_DEFAULTLOCK_H
#if defined(WIN32) || defined(WIN64)
#include "WindowsMutex.h"
#elif defined(__linux__) || defined(__unix__)
#include "PosixMutex.h"
#endif

namespace pipey {
	namespace thread {
		namespace sync {
#if defined(WIN32) || defined(WIN64)
			typedef CWindowsMutex CDefaultLock;
#elif defined(__linux__) || defined(__unix__)
			typedef CPosixMutex CDefaultLock;
#endif
		}
	}
}

#endif
