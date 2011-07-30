#ifndef PIPEY_THREAD_H
#define PIPEY_THREAD_H

#include "../common/common.h"
#include "Executable.h"

namespace pipey {
	namespace thread {

		struct THREAD_INIT {
			IExecutable * pExec;
			void * pParam;
		};

		class IThread {
		  public:
			IThread(void ) { };
			virtual ~IThread(void ) { };

			virtual void Init(const THREAD_INIT & rInit) = 0;

			virtual void Detach() = 0;

			virtual pipey::common::SYNC_RESULT Wait(unsigned long nTimeout = pipey::common::TIME_INFINITE) = 0;
		};
	}
}
#endif
