#ifndef PIPEY_SEMAPHORE_H
#define PIPEY_SEMAPHORE_H


#include "Lock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			/// ancestor abstract class of all the semaphore classes.
			class ISemaphore : virtual public ILock {
			public:
				ISemaphore() { };

				virtual ~ISemaphore() { };

				/// release multiple locks at once.
				virtual void ReleaseMultiple(uint32_t nCount) = 0;

			};

		}
	}
}
#endif
