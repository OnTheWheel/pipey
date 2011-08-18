#ifndef PIPEY_TIMERABLELOCK_H
#define PIPEY_TIMERABLELOCK_H


#include "Lock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			/// ancestor abstract class of all the timed lock classes.
			class ITimerableLock : virtual public ILock {
			  public:
				ITimerableLock() { };

				virtual ~ITimerableLock() { };


				/// acquire a lock, and returns SYNC_TIMEOUT if specified time limit is expired.
				virtual pipey::common::SYNC_RESULT AcquireTimedLock(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE) = 0;

			};

		}
	}
}
#endif
