#ifndef PIPEY_TRIABLELOCK_H
#define PIPEY_TRIABLELOCK_H

#include "Lock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class ITriableLock : virtual public ILock {
			  public:
				ITriableLock() { };

				virtual ~ITriableLock() { };

				virtual pipey::common::SYNC_RESULT TryLock() = 0;

			};

		}
	}
}
#endif
