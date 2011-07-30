#ifndef PIPEY_POSIXTHREAD_H
#define PIPEY_POSIXTHREAD_H

#include <pthread.h>
#include "Thread.h"

namespace pipey {
	namespace thread {

		class CPosixThread : public IThread {
		  public:
			CPosixThread(void );

			virtual ~CPosixThread(void );


		  private:
			pthread_t m_hThread;
			bool m_bInited;
			bool m_bDetached;
			THREAD_INIT m_param;


		  public:
			virtual void Init(const THREAD_INIT & rInit);

			virtual void Detach();

			virtual pipey::common::SYNC_RESULT Wait(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE);

		};

	}
}
#endif
