#ifndef PIPEY_TIMERABLELOCKPTR_H
#define PIPEY_TIMERABLELOCKPTR_H


#include "LockPtr.h"
#include "TimerableLock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class CTimerableLockPtr : virtual public CLockPtr 
			{
			public:
				CTimerableLockPtr(ITimerableLock * pLock);
				virtual ~CTimerableLockPtr();


			private:
				ITimerableLock * m_pLock;

				const CTimerableLockPtr & operator =(const CTimerableLockPtr & rhs) { return *this; };


			public:
				virtual pipey::common::SYNC_RESULT AcquireTimedLock(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE);

			};

		}
	}
}
#endif
