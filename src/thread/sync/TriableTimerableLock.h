#ifndef PIPEY_TRIABLETIMERABLELOCK_H
#define PIPEY_TRIABLETIMERABLELOCK_H


#include "TriableLock.h"
#include "TimerableLock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class ITriableTimerableLock : virtual public ITriableLock, virtual public ITimerableLock {
			public:
				ITriableTimerableLock() { };

				virtual ~ITriableTimerableLock() { };

				virtual pipey::common::SYNC_RESULT AcquireTimedLock(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE) = 0;

				virtual pipey::common::SYNC_RESULT TryLock() = 0;
			};

		}
	}
}
#endif
