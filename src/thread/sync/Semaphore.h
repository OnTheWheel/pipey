#ifndef PIPEY_SEMAPHORE_H
#define PIPEY_SEMAPHORE_H


#include "Lock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class ISemaphore : virtual public ILock {
			public:
				ISemaphore() { };

				virtual ~ISemaphore() { };

				virtual void ReleaseMultiple(unsigned long nCount) = 0;

			};

		}
	}
}
#endif
