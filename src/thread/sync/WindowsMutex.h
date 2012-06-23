#ifndef PIPEY_WINDOWSMUTEX_H
#define PIPEY_WINDOWSMUTEX_H

#include "TriableTimerableLock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class WIN_MUTEX_INIT : public LOCK_INIT {
			public:
				WIN_MUTEX_INIT() :
				lpName(NULL),
				bInitialOwner(false)
				{ };

				WIN_MUTEX_INIT(LPCTSTR name, bool initialOwner) :
				lpName(name),
				bInitialOwner(initialOwner)
				{ };

				LPCTSTR lpName;

				bool bInitialOwner;

			};

			class CWindowsMutex : public ITriableTimerableLock {
			public:
				CWindowsMutex();
				virtual ~CWindowsMutex();


			protected:
				HANDLE m_hMutex;

				bool m_bInited;


			public:
				virtual void Init(const LOCK_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT AcquireLock();

				virtual void ReleaseLock();

				virtual void Close();

				virtual pipey::common::SYNC_RESULT TryLock();

				virtual pipey::common::SYNC_RESULT AcquireTimedLock(uint32_t nMilliSeconds = pipey::common::TIME_INFINITE);

			};

		}
	}
}
#endif
