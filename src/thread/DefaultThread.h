#ifndef PIPEY_DEFAULT_THREAD_H
#define PIPEY_DEFAULT_THREAD_H

#if defined(PIPEY_WINDOWS_SUPPORT)

#include "WindowsThread.h"

namespace pipey {
	namespace thread {
		typedef CWindowsThread CDefaultThread;
	}
}

#elif defined(__linux__) || defined(__unix__)

#include "PosixThread.h"

namespace pipey {
	namespace thread {
		typedef CPosixThread CDefaultThread;
	}
}

#endif

#endif
