#ifndef PIPEY_TRIABLELOCK_H
#define PIPEY_TRIABLELOCK_H

#include "Lock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			/// ancestor abstract class of all the triable lock classes.
			class ITriableLock : virtual public ILock {
			  public:
				ITriableLock() { };

				virtual ~ITriableLock() { };

				/// try to acquire a lock, and returns SYNC_BUSY immediately if the lock is currently dominated by another thread at that moment.
				virtual pipey::common::SYNC_RESULT TryLock() = 0;

			};

		}
	}
}
#endif
