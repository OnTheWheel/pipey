#ifndef PIPEY_POSIXMUTEX_H
#define PIPEY_POSIXMUTEX_H

#include "TriableTimerableLock.h"
#include <pthread.h>

namespace pipey {
	namespace thread {
		namespace sync {

			class POSIX_MUTEX_INIT : public LOCK_INIT {
			public:
				POSIX_MUTEX_INIT() 
				{ };

				//WIN_MUTEX_INIT(LPCTSTR name, bool bnitialOwner)
				//{ };
			};

			class CPosixMutex : public ITriableTimerableLock {
			public:
				CPosixMutex();
				virtual ~CPosixMutex();


			protected:
				pthread_mutex_t m_hMutex;

				bool m_bInited;


			public:
				virtual void Init(const LOCK_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT AcquireLock();

				virtual void ReleaseLock();

				virtual void Close();

				virtual pipey::common::SYNC_RESULT TryLock();

				virtual pipey::common::SYNC_RESULT AcquireTimedLock(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE);

			};

		}
	}
}
#endif
