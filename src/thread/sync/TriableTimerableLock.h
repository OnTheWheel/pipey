#ifndef PIPEY_TRIABLETIMERABLELOCK_H
#define PIPEY_TRIABLETIMERABLELOCK_H


#include "TriableLock.h"
#include "TimerableLock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			/// ancestor abstract class of all the timed and triable lock classes.
			class ITriableTimerableLock : virtual public ITriableLock, virtual public ITimerableLock {
			public:
				ITriableTimerableLock() { };

				virtual ~ITriableTimerableLock() { };

				/// acquire a lock, and returns SYNC_TIMEOUT if specified time limit is expired.
				virtual pipey::common::SYNC_RESULT AcquireTimedLock(uint32_t nMilliSeconds = pipey::common::TIME_INFINITE) = 0;

				/// try to acquire a lock, and returns SYNC_BUSY immediately if the lock is currently dominated by another thread at that moment.
				virtual pipey::common::SYNC_RESULT TryLock() = 0;
			};

		}
	}
}
#endif
