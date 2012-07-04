#ifndef PIPEY_WINDOWSTHREAD_H
#define PIPEY_WINDOWSTHREAD_H

#include "Thread.h"

namespace pipey {
	namespace thread {

		class CWindowsThread : public IThread {
		public:
			CWindowsThread(void );

			virtual ~CWindowsThread(void );


		protected:
			bool m_bInited;
			bool m_bDetached;
			HANDLE m_hThread;

			THREAD_INIT m_param;

		public:
			virtual void Init(const THREAD_INIT & rInit);

			virtual void Detach();

			virtual ::pipey::common::SYNC_RESULT Wait();

		};

	}
}
#endif
