#ifndef PIPEY_OVERLAPPED_EX_H
#define PIPEY_OVERLAPPED_EX_H

#include "WindowsAsyncOperation.h"

namespace pipey {
	namespace io {

		struct OVERLAPPED_EX : public OVERLAPPED
		{
			CWindowsAsyncOperation *pOperation;
			LPWSAOVERLAPPED_COMPLETION_ROUTINE pRoutine;
		};
	}
}

#endif