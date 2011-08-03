#ifndef PIPEY_DEFAULTCONDITION_H
#define PIPEY_DEFAULTCONDITION_H
#if defined(WIN32) || defined(WIN64)
#include "WindowsEvent.h"
#elif defined(__linux__) || defined(__unix__)
#include "PosixCondition.h"
#endif

namespace pipey {
	namespace thread {
		namespace sync {
#if defined(WIN32) || defined(WIN64)
			typedef CWindowsEvent CDefaultCondition;
#elif defined(__linux__) || defined(__unix__)
			typedef CPosixCondition CDefaultCondition;
#endif
		}
	}
}

#endif
