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
			THREAD_INIT m_param;


		  public:
			virtual void Init(const THREAD_INIT & rInit);

			virtual SYNC_RESULT Wait(unsigned long nMilliSeconds = TIME_INFINITE);

			virtual void Close();

		};

	}
}
#endif
