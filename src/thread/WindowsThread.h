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
			HANDLE m_hThread;

			THREAD_INIT m_param;


		  public:
			virtual void Init(const THREAD_INIT & rInit);

			virtual SYNC_RESULT Wait(unsigned long nMilliSeconds = TIME_INFINITE);

			virtual void Close();

		};

	}
}
#endif
