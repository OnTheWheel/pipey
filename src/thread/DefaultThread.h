#ifndef PIPEY_DEFAULT_THREAD_H
#define PIPEY_DEFAULT_THREAD_H

#ifdef WINDOWS_SUPPORT

#include "WindowsThread.h"

namespace pipey {
	namespace thread {
		typedef CWindowsThread CDefaultThread;
	}
}

#endif

#endif