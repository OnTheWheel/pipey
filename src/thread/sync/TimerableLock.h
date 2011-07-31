#ifndef PIPEY_TIMERABLELOCK_H
#define PIPEY_TIMERABLELOCK_H


#include "Lock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class ITimerableLock : virtual public ILock {
			  public:
				ITimerableLock() { };

				virtual ~ITimerableLock() { };

				virtual pipey::common::SYNC_RESULT AcquireTimedLock(unsigned long nMilliSeconds = pipey::common::TIME_INFINITE) = 0;

			};

		}
	}
}
#endif
