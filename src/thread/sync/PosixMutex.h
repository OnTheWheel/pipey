#ifndef PIPEY_POSIXMUTEX_H
#define PIPEY_POSIXMUTEX_H

#include "TriableTimerableLock.h"
#include <pthread.h>

namespace pipey {
	namespace thread {
		namespace sync {

			class POSIX_MUTEX_INIT : public LOCK_INIT {
			public:
				POSIX_MUTEX_INIT() :
#ifdef _POSIX_THREAD_PROCESS_SHARED
				bProcessShared(false),
#endif					
				nType(PTHREAD_MUTEX_DEFAULT)
				{ };

				
#ifdef _POSIX_THREAD_PROCESS_SHARED
				POSIX_MUTEX_INIT(bool processShared, int type) :
				bProcessShared(processShared),
				nType(type)
				{ };
#else
				POSIX_MUTEX_INIT(int type) :
				nType(type)
				{ };
#endif

#ifdef _POSIX_THREAD_PROCESS_SHARED
				bool bProcessShared;
#endif
				int nType;
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

				virtual pthread_mutex_t *GetHandle();

			};

		}
	}
}
#endif
