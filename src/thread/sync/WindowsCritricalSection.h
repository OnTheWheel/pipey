#ifndef PIPEY_WINDOWSCRITRICALSECTION_H
#define PIPEY_WINDOWSCRITRICALSECTION_H

#include "TriableLock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class CWindowsCritricalSection : 
#if _WIN32_WINNT >= 0x0400
				public ITriableLock 
#else
				public ILock 
#endif
			{
			public:
				CWindowsCritricalSection();

				virtual ~CWindowsCritricalSection();


			protected:
				CRITICAL_SECTION m_cs;

				bool m_bInited;


			public:
				virtual void Init(const LOCK_INIT * pParam = NULL);

				virtual pipey::common::SYNC_RESULT AcquireLock();

				virtual void ReleaseLock();

				virtual void Close();

#if _WIN32_WINNT >= 0x0400
				virtual pipey::common::SYNC_RESULT TryLock();
#endif

			};

		}
	}
}
#endif
