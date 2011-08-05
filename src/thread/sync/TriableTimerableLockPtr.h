#ifndef PIPEY_TRIABLETIMERABLELOCKPTR_H
#define PIPEY_TRIABLETIMERABLELOCKPTR_H

#include "TriableLockPtr.h"
#include "TimerableLockPtr.h"
#include "TriableTimerableLock.h"

namespace pipey {
	namespace thread {
		namespace sync {

			class CTriableTimerableLockPtr : public CTriableLockPtr, public CTimerableLockPtr {
			public:
				CTriableTimerableLockPtr(ITriableTimerableLock * pLock);

				virtual ~CTriableTimerableLockPtr();


			private:
				const CTriableTimerableLockPtr & operator =(const CTriableTimerableLockPtr & rhs) { return *this; };

			};

		}
	}
}

#endif
