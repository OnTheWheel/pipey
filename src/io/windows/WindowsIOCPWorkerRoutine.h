#ifndef PIPEY_WINDOWSIOCPWORKERROUTINE_H
#define PIPEY_WINDOWSIOCPWORKERROUTINE_H

#include "../../common/common.h"
#include "../../thread/Executable.h"

namespace pipey {
	namespace io {
		class CWindowsIOCPWorkerRoutine : public pipey::thread::IExecutable {
		public:
			CWindowsIOCPWorkerRoutine() { };
			virtual ~CWindowsIOCPWorkerRoutine() { };

			virtual void Execute(void * pParam);

		private:
			HANDLE m_hIOCP;
		};
	}
}

#endif